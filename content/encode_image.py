from PIL import Image
from PIL import ImageColor
import argparse as ap
import numpy
import sys
import math 

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

def get_image(image_path):
    """Get a numpy array of an image so that one can access values[y][x]."""
    image = Image.open(image_path, "r")
    width, height = image.size
    pixel_values = list(image.getdata())
    if (image.mode == "RGB"):
        for idx, px in enumerate(pixel_values):
            pixel_values[idx] = [px[0], px[1], px[2], 255]
    elif image.mode != "RGBA":
        print("Incorrect Mode: %s" % image.mode)
        raise None
    return (list(chunks(pixel_values, width)), width, height)

def is_transparent(pixel):
    return pixel[3]<1

def find_alpha_run(data):
    length = 0
    for item in data:
        if is_transparent(item):
            length += 1
        else:
            break
    return length

def find_alpha_prelude(row):
    return find_alpha_run(row)

def find_alpha_epilogue(row):
    return find_alpha_run(reversed(row))

def find_mid_alpha(row):
    count = 0
    for item in row:
        if is_transparent(item):
            count += 1
    return count

def apply_alpha_mask(rows, alpha_mask):
    for ridx, row in enumerate(rows):
        for pidx, px in enumerate(row):
            if (px == alpha_mask):
                row[pidx] = [0,0,0,0]

def encode_pixel16(pixel):
    alpha = 1 << 5
    if pixel[3] == 0:
        alpha = 0
    color = ((pixel[0] >> 3) << 0) | ((pixel[1] >> 3) << 6) | ((pixel[2] >> 3) << 11) | alpha
    return color

def format_pixel_data(rows, print, width, buf_width, prefix, metadata):
    
    

    buffer=prefix
    for idx, row in enumerate(rows):
        alpha_prelude = find_alpha_prelude(row)
        alpha_epilogue = width - find_alpha_epilogue(row[alpha_prelude:]);
        mid_alpha = find_mid_alpha(row[alpha_prelude:-alpha_epilogue])

        metadata.append(alpha_epilogue & 0xFFFF)
        metadata.append(((alpha_prelude & 0x7FFF)) | ((mid_alpha != 0) << 15))

        for pixel in row:
            alpha = 1 << 5
            if pixel[3] == 0:
                alpha = 0
            color = ((pixel[0] >> 3) << 0) | ((pixel[1] >> 3) << 6) | ((pixel[2] >> 3) << 11) | alpha
            buffer+=hex(color) + ","
            if len(buffer) >= buf_width:
                print(buffer)
                buffer=prefix
        print(buffer)
        buffer=prefix
        print()
    if buffer != prefix:
        print(buffer)

def format_metadata(metadata, print, buf_width, prefix):
    buffer=prefix
    for m in metadata:
        buffer+=hex(m) + ","
        if len(buffer) >= buf_width:
                print(buffer)
                buffer=prefix
    if buffer != prefix:
        print(buffer)

def format_c(name, rows, print):

    buf_width=140
    metadata = []

    height=len(rows)
    width=len(rows[0])

    print("#include <stdint.h>")
    print("#include \"sprite.h\"")
    print()
    print(f"static uint16_t __attribute__((aligned(4))) {name}_pixels_{width}x{height}[] = {{")
    format_pixel_data(rows, print, width, buf_width, "    ", metadata)
    print("};")
    print()
    print(f"static uint16_t __attribute__((aligned(4))) {name}_metadata_{width}x{height}[] = {{")
    format_metadata(metadata, print, buf_width, "    ")
    print("};")
    print()
    print(f"static const image_data_t {name}_{width}x{height} = {{")
    print(f"    .pixels = {name}_pixels_{width}x{height},")
    print(f"    .metadata = (uint32_t*){name}_metadata_{width}x{height}")
    print("};")

output = None
def fileprint(input=""):
    output.write(input)
    output.write("\n")

def main():
    parser = ap.ArgumentParser(prog="Sprite Image Encoder", description='Convert an image into 16 bit sprite data with metadata')
    parser.add_argument("input_path", help='Input file path')
    parser.add_argument("output_path", help='Output file path')
    parser.add_argument("name", help='buffer name')
    args = parser.parse_args()

    print(args.input_path)
    (im, width, height) = get_image(args.input_path)

    global output
    output = open(args.output_path, "w", encoding="utf-8", newline='\n')

    format_c(args.name, im, fileprint)

if __name__ == "__main__":
    main()