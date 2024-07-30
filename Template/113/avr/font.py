from PIL import Image, ImageDraw, ImageFont
import numpy as np


def text_to_bitmap(text, font_path="unifont.otf", font_size=16):
    try:
        font = ImageFont.truetype(font_path, font_size)
    except OSError:
        print(f"無法打開字體文件: {font_path}")
        return {}

    bitmaps = {}

    for char in text:
        # Create a new image with white background
        image = Image.new("1", (font_size, font_size), color=1)
        draw = ImageDraw.Draw(image)

        # Draw the character on the image
        draw.text((0, 0), char, font=font, fill=0)

        # Convert image to numpy array
        bitmap = np.array(image)

        # Get hex value of the character
        char_code = ord(char)

        # Convert bitmap to horizontal scan binary array
        binary_array = []
        for row in bitmap:
            binary_row = 0
            for i, pixel in enumerate(row):
                binary_row = (binary_row << 1) | (0 if pixel else 1)
                if (i + 1) % 8 == 0:
                    binary_array.append(binary_row)
                    binary_row = 0
            if font_size % 8 != 0:
                binary_array.append(binary_row)

        bitmaps[char_code] = (char, binary_array)

    return bitmaps


def write_bitmaps_to_file(bitmaps, filename="font.h"):
    with open(filename, "w", encoding="utf-8") as f:
        for char_code, (char, bitmap) in bitmaps.items():
            f.write(f"static const unsigned char PROGMEM U{char_code:X}[] = {{  // {char}\n  ")
            for i, value in enumerate(bitmap):
                if i % 8 == 0 and i != 0:
                    f.write("\n  ")
                f.write(f"0x{value:02X}, ")
            f.write("\n};\n\n")

        f.write("typedef struct {\n")
        f.write("  const char* character;\n")
        f.write("  const unsigned char* bitmap;\n")
        f.write("  uint8_t width;\n")
        f.write("  uint8_t height;\n")
        f.write("} CharBitmap;\n\n")
        f.write("CharBitmap charBitmaps[] = {\n")
        for char_code, (char, bitmap) in bitmaps.items():
            f.write(f'  {{"{char}", U{char_code:X}, 16, 16}},\n')
        f.write("};\n")


if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print("用法: python font.py -t <中文全形文字>")
        sys.exit(1)

    if sys.argv[1] != "-t" or len(sys.argv) < 3:
        print("用法: python font.py -t <中文全形文字>")
        sys.exit(1)

    text = sys.argv[2]
    bitmaps = text_to_bitmap(text)
    write_bitmaps_to_file(bitmaps)
    print("點陣圖已輸出到 font.h 文件")
