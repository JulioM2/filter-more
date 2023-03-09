#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Declaring a new structure to make an array capable of receibe each pixel RGB value
typedef struct
{
    float rgbtRed;
    float rgbtBlue;
    float rgbtGreen;
} RGBINT;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE swap_list[width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            swap_list[j] = image[i][width - (j + 1)];
        }
        for (int j = 0; j < width; j++)
        {
            image[i][j] = swap_list[j];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_copy[height][width];
    float ave_div = 0.0;
    int heig, widt,average_red = 0, average_blue = 0, average_green = 0;
    for (int i = 0; i < height; i++)
    {
        if (i >= 1)
        {
            heig = i - 1;
        }
        else
        {
            heig = i;
        }
        for (int j = 0; j < width; j++)
        {
            if (j >= 1)
            {
                widt = j - 1;
            }
            else
            {
                widt = j;
            }
            for (int a = heig; a < i + 2; a++)
            {
                for (int b = widt; b < j + 2; b++)
                {
                    average_red += image[a][b].rgbtRed;
                    average_green += image[a][b].rgbtGreen;
                    average_blue += image[a][b].rgbtBlue;
                    ave_div += 1.0;
                    if (b + 2 > width)
                    {
                        break;
                    }
                }
                if (a + 2 > height)
                {
                    break;
                }
            }
            image_copy[i][j].rgbtRed = round(average_red / ave_div);
            image_copy[i][j].rgbtGreen = round(average_green / ave_div);
            image_copy[i][j].rgbtBlue = round(average_blue / ave_div);
            ave_div = 0.0;
            average_red = 0;
            average_green = 0;
            average_blue = 0;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Arrays copies to receive the new values of original image array
    RGBINT image_copy_gx[height][width], image_copy_gy[height][width];
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}, Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int heig, widt, h_v, w_v;
    // Loop to pass by each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Verify if there's a value before current i at the array
            if (i >= 1)
            {
                heig = i - 1;
                h_v = 0;
            }
            else
            {
                heig = i;
                h_v = 1;
            }
            // Verify if there's a value before current j at the array
            if (j >= 1)
            {
                widt = j - 1;
                w_v = 0;
            }
            else
            {
                widt = j;
                w_v = 1;
            }
            // Loop to pass by each RGB value and multiply by it's Gx and Gy corresponding
            for (int a = heig; a < i + 2; a++)
            {
                for (int b = widt; b < j + 2; b++)
                {
                    image_copy_gx[i][j].rgbtRed += Gx[h_v][w_v] * image[a][b].rgbtRed;
                    image_copy_gx[i][j].rgbtGreen += Gx[h_v][w_v] * image[a][b].rgbtGreen;
                    image_copy_gx[i][j].rgbtBlue += Gx[h_v][w_v] * image[a][b].rgbtBlue;
                    image_copy_gy[i][j].rgbtRed += Gy[h_v][w_v] * image[a][b].rgbtRed;
                    image_copy_gy[i][j].rgbtGreen += Gy[h_v][w_v] * image[a][b].rgbtGreen;
                    image_copy_gy[i][j].rgbtBlue += Gy[h_v][w_v] * image[a][b].rgbtBlue;
                    // Update width value of Gx and Gy matrix
                    w_v += 1;
                    // Break if it the actual value is bigger than the width limit
                    if (b + 2 > width)
                    {
                        break;
                    }
                }
                // Also break if the actual value is bigger than height limit
                if (a + 2 > height)
                {
                    break;
                }
                // Also updating the width value for Gx and Gy matrix
                if (j >= 1)
                {
                    w_v = 0;
                }
                else
                {
                    w_v = 1;
                }
                // Update height value of Gx and Gy matrix
                h_v += 1;
            }
        }
    }
    // Final loop to pass each new value to the original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the square root of the squared values, round it and cap the value to 255 if it is greater
            image[i][j].rgbtRed = fmin(round(sqrt((image_copy_gx[i][j].rgbtRed * image_copy_gx[i][j].rgbtRed) +
                                                  (image_copy_gy[i][j].rgbtRed * image_copy_gy[i][j].rgbtRed))), 255);
            image[i][j].rgbtGreen = fmin(round(sqrt((image_copy_gx[i][j].rgbtGreen * image_copy_gx[i][j].rgbtGreen) +
                                                    (image_copy_gy[i][j].rgbtGreen * image_copy_gy[i][j].rgbtGreen))), 255);
            image[i][j].rgbtBlue = fmin(round(sqrt((image_copy_gx[i][j].rgbtBlue * image_copy_gx[i][j].rgbtBlue) +
                                                   (image_copy_gy[i][j].rgbtBlue * image_copy_gy[i][j].rgbtBlue))), 255);
        }
    }
    return;
}
