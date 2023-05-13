#include "bitmap.h"

std::unique_ptr<Bitmap> readBmpFile(const char *path)
{
    FILE *file = std::fopen(path, "r");
    if (!file)
    {
        std::perror("File opening failed");
        return nullptr;
    }

    if (access(path, R_OK))
    {
        std::perror("File reading permission denied");
        fclose(file);
        return nullptr;
    }

    std::unique_ptr<Bitmap> bitmap = std::make_unique<Bitmap>();

    fread(&(bitmap->bfType), 1, 2, file);

    if (0x4D42 != bitmap->bfType)
    {
        std::perror("Not a Bitmap file");
        fclose(file);
        return nullptr;
    }

    fseek(file, 10, SEEK_SET);
    fread(&(bitmap->bfOffBits), 1, 4, file);

    fseek(file, 18, SEEK_SET);
    fread(&(bitmap->biWidth), 1, 4, file);

    fread(&(bitmap->biHeight), 1, 4, file);

    std::shared_ptr<uint8_t[]> data = std::make_shared<uint8_t[]>(static_cast<size_t>(bitmap->biWidth * bitmap->biHeight * 3));
    fseek(file, bitmap->bfOffBits, SEEK_SET);
    fread(data.get(), sizeof(uint8_t), static_cast<size_t>(bitmap->biWidth * bitmap->biHeight * 3), file);
    bitmap->data = data;

    bitmap->size = bitmap->biHeight * bitmap->biWidth * 3;

    fclose(file);

    return bitmap;
}

bool writeBmpFile(const char *path, uint8_t *buffer, size_t size, long int offset, int whence)
{
    FILE *file = std::fopen(path, "r+");
    if (!file)
    {
        std::perror("File opening failed");
        return false;
    }

    if (access(path, W_OK))
    {
        std::perror("File writing permission denied");
        fclose(file);
        return false;
    }
    if (fseek(file, offset, whence) != 0)
    {
        std::perror("Fseek failed");
        return false;
    }

    fseek(file, offset, whence);
    fwrite(buffer, sizeof(uint8_t), size, file);
    fclose(file);
    return true;
}

bool copyBitmapHeader(const char* in, const char* out)
{
    FILE *infile = std::fopen(in, "r");
    FILE *outfile = std::fopen(out, "w");

    if (!infile || !outfile)
    {
        std::perror("Input or output File opening failed");
        return false;
    }

    if (access(out, R_OK) || access(out, W_OK))
    {
        std::perror("File reading or writing permission denied");
        fclose(infile);
        fclose(outfile);
        return false;
    }

    struct stat s;
    if (stat(in, &s) < 0)
    {
        std::perror("Stat error");
        return false;
    }

    uint16_t bfType;
    uint32_t bfOffBits;
    fread(&(bfType), 1, 2, infile);

    if (0x4D42 != bfType)
    {
        std::perror("Not a Bitmap file");
        fclose(infile);
        fclose(outfile);
        return false;
    }

    fseek(infile, 10, SEEK_SET);
    fread(&(bfOffBits), 1, 4, infile);

    fseek(infile, 0, SEEK_SET);
    std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(bfOffBits - 1);
    fread(buffer.get(), sizeof(uint8_t), bfOffBits - 1, infile);
    fwrite(buffer.get(), sizeof(uint8_t), bfOffBits - 1, outfile);

    fclose(infile);
    fclose(outfile);

    return true;
}
