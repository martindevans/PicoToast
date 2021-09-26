import argparse as ap
import json

#spawnX;SpawnY;
#numboxes;
#box xpos;box ypos;box width;box height;...box xpos;box ypos;box width;box height;
#nummelons;
#melon xpos;melon ypos;...melon xpos;melon ypos;
#enemyID;boolean for visible or not visible;

output = None

def print(input=""):
    output.write(input)
    output.write("\n")

def main():
    parser = ap.ArgumentParser(prog="T Level Converter Estimator", description='Convert a T level into a C header file')
    parser.add_argument("input_path", help='Input file path')
    parser.add_argument("level_name", help='Level Name')
    args = parser.parse_args()

    name = args.level_name
    input = open(args.input_path, "r").read().strip().split(";")
    while '' in input:
        input.remove('')

    spawnX = float(input[0])
    spawnY = float(input[1])
    numboxes = int(input[2])
    nummelons = int(input[3 + numboxes * 4])

    boxes = []
    for i in range(numboxes):
        base_addr = 3 + i * 4
        boxes.append([
            float(input[base_addr + 0]),
            float(input[base_addr + 1]),
            float(input[base_addr + 2]),
            float(input[base_addr + 3]),
            i == 0,
            False
        ])

    melons = []
    for i in range(nummelons):
        base_addr = 4 + numboxes * 4 + i * 2
        melons.append([
            float(input[base_addr + 0]),
            float(input[base_addr + 1]),
        ])

    base_enemy_addr = 4 + numboxes * 4 + nummelons * 2
    input = input[base_enemy_addr:]
    numenemies = int(len(input) / 2)
    for i in range(numenemies):
        idx = int(input[i * 2 + 0]) - 1
        present = json.loads(input[i * 2 + 1].lower())
        boxes[idx][5] = present

    global output
    output = open(f"{name}.h", "w", encoding="utf-8", newline='\n')

    print(f"#ifndef _{name}_H")
    print(f"#define _{name}_H")
    print()
    print("#include \"level.h\"")
    print()
    print(f"box_t {name}_boxes[] = {{")
    for box in boxes:
        print("    {")
        print(f"        .x = {int(box[0])},")
        print(f"        .y = {int(box[1])},")
        print(f"        .w = {int(box[2])},")
        print(f"        .h = {int(box[3])},")
        print(f"        .exit_zone = {str(box[4]).lower()},")
        print(f"        .has_enemy = {str(box[5]).lower()}")
        print("    },")
    print("};")
    print()
    print(f"melon_t {name}_melons[] = {{");
    for melon in melons:
        print("    {")
        print(f"        .x = {melon[0]},")
        print(f"        .y = {melon[1]}")
        print("    },")
    print("};")
    print()
    print(f"level_t {name} = {{")
    print(f"    .spawnX = {spawnX},")
    print(f"    .spawnY = {spawnY},")
    print(f"    .numboxes = {numboxes},")
    print(f"    .boxes = &{name}_boxes[0],")
    print(f"    .nummelons = {nummelons},")
    print(f"    .melons = &{name}_melons[0],")
    print("};")
    print()
    print("#endif")

main()