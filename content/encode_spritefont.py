from PIL import Image
from PIL import ImageColor
import argparse as ap
import numpy
import sys
import math 

import encode_image

def chunks(lst, n):
    """Yield successive n-sized chunks from lst."""
    for i in range(0, len(lst), n):
        yield lst[i:i + n]

def get_image(image_path):
    """Get a numpy array of an image so that one can access values[y][x]."""
    image = Image.open(image_path, "r")
    width, height = image.size
    pixel_values = list(image.getdata())

    if (image.mode != "RGBA"):
        image = image.convert("RGB")
        pixel_values = list(image.getdata())
        for idx, px in enumerate(pixel_values):
            pixel_values[idx] = [px[0], px[1], px[2], 255]

    return (list(chunks(pixel_values, width)), width, height)

def find_markers(img_data, marker_colour, bg_colour):
    # Each character has a `marker_colour` pixel in the top left corner, search along for those
    # Once found, replace them with the BG colour
    markers = []
    marker = list(marker_colour)
    marker.append(255)
    for idx, px in enumerate(img_data[0]):
        if (px[:3] == marker[:3]):
            markers.append(idx)
            img_data[0][idx] = bg_colour

    return markers

def replace_background(img_data, width, height, bg_colour):
    for x in range(width):
        for y in range(height):
            if (img_data[y][x][:3] == bg_colour[:3]):
                img_data[y][x] = [0, 0, 0, 0]
    return img_data

def alpha_threshold(img_data, width, height, threshold):
    for x in range(width):
        for y in range(height):
            px = img_data[y][x]
            if (px[3] < threshold):
                px[3] = 0
            else:
                px[3] = 255
            img_data[y][x] = px
    return img_data

def encode_pixels(img_data, width, height):
    for x in range(width):
        for y in range(height):
            px = img_data[y][x]
            img_data[y][x] = encode_image.encode_pixel16(px)
    return img_data

def extract_chars(img_data, img_width, img_height, markers):
    characters = []
    offset = 0
    for idx, marker in enumerate(markers):
        next_marker = -1
        if (idx + 1 == len(markers)):
            next_marker = img_width
        else:
            next_marker = markers[idx + 1]
        chr_width = next_marker - marker
        chr_height = img_height

        data = []
        for h in range(chr_height):
            data.append(img_data[h][offset:(offset + chr_width)])

        characters.append((data, chr_width, chr_height))
        offset += chr_width

    return characters

output = None
def fileprint(input=""):
    output.write(str(input))
    output.write("\n")

def format_c(characters, name, print, first_char, last_char, height):
    # At the start is an offset table, this points to the offset in the main
    # data table for each character
    # uint16_t char_offset = offset_table[(character - base_character) * 2 * height + y * 2];

    bytes_used = 0
    metadata = []
    current_char = first_char

    print("#include <stdint.h>")
    print()
    print(f"static uint16_t font_{name}_pixels[] = {{")

    px_offset = 0
    for idx, character in enumerate(characters):
        (char_pixels, width, height) = character
        print(f"    // {current_char} `{chr(current_char)}`")
        row_offsets = []
        row_lengths = []
        row_discontinuous = []
        for y in range(height):
            row = char_pixels[y]

            # Count transparent pixels in row (at start, at end, anywhere in middle)
            true_length = len(row)
            prelude = encode_image.find_alpha_prelude(row)
            epilogue = encode_image.find_alpha_epilogue(row)
            if (prelude == len(row)):
                epilogue = 0
            mid = encode_image.find_mid_alpha(row) - prelude - epilogue

            # Remove prelude and epilogue from row
            del row[0:prelude]
            del row[len(row)-epilogue:]

            # Sanity check that index calculation is correct
            expected_meta_index = (current_char - first_char) * 2 * height + y * 2
            if (expected_meta_index != len(metadata)):
                raise Exception(f"Incorrect index! Expected:{len(metadata)} Calculated:{expected_meta_index}")

            # Check that all the metadata can fit into the given number of bits
            if len(row) > 127:
                raise Exception("len(row) is too large")
            if (true_length > 255):
                raise Exception("true_length is too large!")
            if (px_offset > 4095):
                raise Exception("px_offset is too large!")
            if (prelude > 15):
                raise Exception("prelude is too large!")
            
            # XXXXXXXXXXXX XXXX
            # ^ 12 bits: pixel offset (location of the first pixel for this row of this character)
            #              ^ 4 bits: alpha prelude length
            metadata.append((px_offset << 4) | prelude);

            # XXXXXXX X XXXXXXXX
            # ^ 7 bits: Number of pixels to copy
            #         ^ 1 bit: is the pixel span discontinuous
            #           ^ 8 bits: number of pixels to move the cursor over to draw the next character
            discontinuous = int(mid > 0) << 8
            metadata.append((len(row) << 9) | discontinuous | true_length)

            # Prints out pixel data
            buffer = "    "
            for px in row:
                px = encode_image.encode_pixel16(px)
                buffer += "0x" + hex(px)[2:].zfill(4).upper() + ","
                px_offset += 1
                bytes_used += 2
            if (len(row) == 0):
                buffer += "// <-- Blank Row -->"
            print(buffer)

        current_char += 1
        print()
    print("};")
    print()

    print(f"static uint16_t font_{name}_metadata[] = {{")
    for chunk in chunks(metadata, 16):
        buffer = "    "
        for item in chunk:
            buffer += "0x" + hex(item)[2:].zfill(4).upper() + ","
            bytes_used += 2
        print(buffer)
    print("};")
    print()
    print(f"// Bytes Used: {bytes_used}")

    # Sanity check that we reached the specified final character
    if (current_char - 1 != last_char):
        raise Exception("Did not reach last character!")


    offsets = {}
    offsets[1] = 2

def main():
    parser = ap.ArgumentParser(description='Convert an image into 16 bit sprite font')
    parser.add_argument("input_path", help='Input file path')
    parser.add_argument("output_path", help='Output file path')
    parser.add_argument("name", help='buffer name')
    parser.add_argument("bgcol", help='Background colour (will be replaced with transparent pixels)')
    parser.add_argument("marker", help='Marker colour (marks top-left corner of each character)')
    parser.add_argument("first_char", help='First character as ASCII ordinal', type=int)
    parser.add_argument("last_char", help='Last character as ASCII ordinal', type=int)
    args = parser.parse_args()

    first_char = chr(args.first_char)
    last_char = chr(args.last_char)
    marker_colour = list(ImageColor.getcolor(args.marker, "RGB"))
    marker_colour.append(255)
    background_colour = list(ImageColor.getcolor(args.bgcol, "RGB"))
    background_colour.append(255)

    global output
    output = open(args.output_path, "w", encoding="utf-8", newline='\n')

    ## Get data as RGBA
    (image_data, width, height) = get_image(args.input_path)

    ## Find the marker pixels (return list of indices) and replace them all with BG colour
    markers = find_markers(image_data, marker_colour, background_colour)

    ## Replace all BG coloured pixels with transparent black
    image_data = replace_background(image_data, width, height, background_colour)

    ## Apply alpha threshold (either completely transparent or completely opaque)
    image_data = alpha_threshold(image_data, width, height, 0.5)

    ## Extract individual characters to their own arrays
    characters = extract_chars(image_data, width, height, markers)

    ## Write to output file
    format_c(characters, args.name, fileprint, args.first_char, args.last_char, height)

if __name__ == "__main__":
    main()