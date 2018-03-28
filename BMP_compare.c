#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>

/******************定义位图RGB结构体*******************/
typedef struct
{
    BYTE rgbRed;
    BYTE rgbGreen;
    BYTE rgbBlue;
} RGB;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void show(char inPicName_1[20], char inPicName_2[20], char outPicName[20]);
void picCmp(char inPicName_1[20], char inPicName_2[20], char outPicName[20]);

void main()
{
    char inPicName_1[20], inPicName_2[20], outPicName[20];
    show(inPicName_1, inPicName_2, outPicName);
    picCmp(inPicName_1, inPicName_2, outPicName);
    system("pause");
}

void show(char inPicName_1[20], char inPicName_2[20], char outPicName[20])
{
    printf("\n\n\n\t\t\t\t\t\tXXXX图片对比程序\n\n");
    printf("\t\t\t_________________________________________________________________\n");
    printf("\n\t\t\t   ====>请输入第一张图片名称：");
    scanf("%s", inPicName_1);
    printf("\n\t\t\t   ====>请输入第二张图片名称：");
    scanf("%s", inPicName_2);
    printf("\n\t\t\t   ====>请输入输出图片名称：");
    scanf("%s", outPicName);
}

void picCmp(char inPicName_1[20], char inPicName_2[20], char outPicName[20])
{
    RGB **picRgb1, **picRgb2;                        //定义图片RGB数组
    BITMAPFILEHEADER inFileHeader_1, inFileHeader_2; //定义头文件
    BITMAPINFOHEADER inInfoHeader_1, inInfoHeader_2; //定义为图信息
    FILE *inPic_1, *inPic_2;                         //定义输入图片一、二文件
    FILE *outPic;                                    //定义输出图片文件
    int width, height;                               //定义图片宽、高
    int red, green, blue;                            //定义RGB差值
    int i, j;
    int value = 20;

    if ((inPic_1 = fopen(inPicName_1, "rb")) == NULL) //打开第一张图片文件
        printf("pic_1 input filed!\n");
    if ((inPic_2 = fopen(inPicName_2, "rb")) == NULL) //打开第二张图片文件
        printf("pic_2 input filed!\n");

    fread(&inFileHeader_1, sizeof(BITMAPFILEHEADER), 1, inPic_1);
    fread(&inFileHeader_2, sizeof(BITMAPFILEHEADER), 1, inPic_2); //读取头文件
    fread(&inInfoHeader_1, sizeof(BITMAPINFOHEADER), 1, inPic_1);
    fread(&inInfoHeader_2, sizeof(BITMAPINFOHEADER), 1, inPic_2); //读取位图信息

    width = inInfoHeader_1.biWidth;   //图片宽度
    height = inInfoHeader_1.biHeight; //图片高度

    picRgb1 = (RGB **)malloc(sizeof(RGB *) * width); //定义储存图片一的RGB数组
    for (i = 0; i < width; i++)
        picRgb1[i] = (RGB *)malloc(sizeof(RGB) * height);

    picRgb2 = (RGB **)malloc(sizeof(RGB *) * width); //定义储存图片二的RGB数组
    for (i = 0; i < width; i++)
        picRgb2[i] = (RGB *)malloc(sizeof(RGB) * height);

    for (i = 0; i < width; i++)
        for (j = 0; j < height; j++)
        {
            fread(&picRgb1[i][j], sizeof(RGB), 1, inPic_1); //读取图片一RGB
            fread(&picRgb2[i][j], sizeof(RGB), 1, inPic_2); //读取图片二RGB

            red = abs(picRgb1[i][j].rgbRed - picRgb2[i][j].rgbRed); //计算RGB差值
            green = abs(picRgb1[i][j].rgbGreen - picRgb2[i][j].rgbGreen);
            blue = abs(picRgb1[i][j].rgbBlue - picRgb2[i][j].rgbBlue);

            if (red > value || green > value || blue > value)                                //比较输入图片一、二RGB
                picRgb1[i][j].rgbRed = picRgb1[i][j].rgbGreen = picRgb1[i][j].rgbBlue = 255; //白
        }

    if ((outPic = fopen(outPicName, "wb")) == NULL) //创建输出图片
        printf("pic output filed!\n");

    fwrite(&inFileHeader_1, sizeof(BITMAPFILEHEADER), 1, outPic); //写入头文件
    fwrite(&inInfoHeader_1, sizeof(BITMAPINFOHEADER), 1, outPic); //写入位图信息
    for (i = 0; i < width; i++)
        for (j = 0; j < height; j++)
            fwrite(&picRgb1[i][j], sizeof(RGB), 1, outPic); //写入RGB

    fclose(outPic); //关闭输出图片文件

    for (i = 0; i < width; i++) //释放储存图片一的数组
        free(picRgb1[i]);
    free(picRgb1);

    for (i = 0; i < width; i++) //释放储存图片二的数组
        free(picRgb2[i]);
    free(picRgb2);

    fclose(inPic_1); //关闭输入图片一文件
    fclose(inPic_2); //关闭输入图片二文件
}
