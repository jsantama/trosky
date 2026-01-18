import sys
import os
from PIL import Image

def convert_image(image_path, output_header_path, width=40, height=40):
    try:
        im = Image.open(image_path)
        im = im.resize((width, height), Image.Resampling.LANCZOS)
        im = im.convert('RGB')
        
        pixels = list(im.getdata())
        
        with open(output_header_path, 'w') as f:
            f.write("#ifndef TROSKY_BITMAP_H\n")
            f.write("#define TROSKY_BITMAP_H\n\n")
            f.write("#include <Arduino.h>\n")
            f.write("#include <pgmspace.h>\n\n")
            f.write(f"#define TROSKY_WIDTH {width}\n")
            f.write(f"#define TROSKY_HEIGHT {height}\n\n")
            f.write(f"const uint16_t troskyBitmap[{width * height}] PROGMEM = {{\n")
            
            for i, p in enumerate(pixels):
                # RGB565 conversion
                r = (p[0] >> 3) & 0x1F
                g = (p[1] >> 2) & 0x3F
                b = (p[2] >> 3) & 0x1F
                rgb565 = (r << 11) | (g << 5) | b
                
                f.write(f"0x{rgb565:04X}, ")
                if (i + 1) % 12 == 0:
                    f.write("\n")
            
            f.write("\n};\n\n")
            f.write("#endif\n")
            
        print(f"Successfully converted {image_path} to {output_header_path}")
        return True
    except Exception as e:
        print(f"Error converting image: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python img2header.py <input_image> <output_header>")
        sys.exit(1)
        
    convert_image(sys.argv[1], sys.argv[2])
