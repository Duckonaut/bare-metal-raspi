#include <common/io.h>
#include <common/util.h>
#include <kernel/graphics.h>
#include <stdarg.h>

static pixel_t background = {{ 0, 0, 0 }};
static pixel_t foreground = {{ 255, 255, 255 }};

#define PUTC(c) gpu_putc(c, background, foreground)
#define PUTS(s) gpu_puts(s, background, foreground)

/* Changing colors
 * syntax: %[foreground:background]
 * foreground: uint32_t (RRGGBBAA) in hex
 * background: uint32_t (RRGGBBAA) in hex
 *
 * examples: %[FFFFFF00:00000000]
 *           %[FF00FF00:_]
 *           %[_:FF00FF00]
 *           %[_:_]
 *
 * AA is ignored for now
 * _ to discard change and leave current color
 * 
 * returns the jump needed to keep on track
 */
static uint32_t try_color_change(char* s) {
    char buf[19];
    buf[0] = '[';
    uint32_t skip = 0;

    uint32_t valid_brackets = 0;
    for (int i = 1; i < 19; i++) {
        char c = s[i];
        if (c == ']') {
            buf[i] = ']';
            valid_brackets = 1;
            skip++;
            break;
        }
        else if (c == '\0') {
            buf[i] = '\0';
            return 0;
            break;
        }
        else if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || c == '_' || c == ':')) {
            return 0;
            break;
        }
        else {
            buf[i] = c;
            skip++;
        }
    }

    if (!valid_brackets) {
        return 0;
    }

    uint32_t ignoreFirst = 0;
    if (buf[1] == '_') {
        ignoreFirst = 1;
    }

    if (ignoreFirst && buf[2] != ':') {
        return 0;
    }
    else if (!ignoreFirst && buf[9] != ':') {
        return 0;
    }

    uint32_t ignoreSecond = 0;
    if (ignoreFirst && buf[3] == '_') {
        ignoreSecond = 1;
    }
    else if (!ignoreFirst && buf[10] == '_') {
        ignoreSecond = 1;
    }

    char nbuf[8];
    
    if (!ignoreFirst) {
        for (int i = 0; i < 8; i++) {
            nbuf[i] = buf[1 + i];
        }
        
        foreground.r = xtob(nbuf);
        foreground.g = xtob(nbuf + 2);
        foreground.b = xtob(nbuf + 4);
    }

    if (!ignoreSecond) {
        uint32_t start = ignoreFirst ? 3 : 10;

        for (int i = 0; i < 8; i++) {
            nbuf[i] = buf[start + i];
        }
        
        background.r = xtob(nbuf);
        background.g = xtob(nbuf + 2);
        background.b = xtob(nbuf + 4);
    }


    return skip + 1;
}

void printf(char* s, ...) {
    va_list args;
    uint32_t skip;

    va_start(args, s);

    while(*s) {
        char c = *s++;

        // If we have a format specifier
        if (c == '%') {
            char t = *s++;

            switch (t) {
                case '%':
                    PUTC('%');
                    break;
                case 'd':
                    PUTS(itoa(va_arg(args, uint32_t), 10));
                    break;
                case 'x':
                    PUTS(itoa(va_arg(args, uint32_t), 16));
                    break;
                case 's':
                    PUTS(va_arg(args, char*));
                    break;
                case '[':
                    skip = try_color_change(s - sizeof(char));
                    if (!skip) {
                        PUTC('%');
                        PUTC('[');
                    }
                    else {
                        s += skip - 1;
                    }
                    break;
                default:
                    PUTC('%');
                    PUTC(t);
                    break;
            }
        }
        else {
           PUTC(c); 
        }
    }
}
