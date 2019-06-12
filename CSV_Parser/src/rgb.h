/*
 * rgb.h
 *
 *  Created on: 10 cze 2019
 *      Author: Bartosz Komar
 */

#ifndef RGB_H_
#define RGB_H_



struct RGB
   {
      unsigned char mRed;
      unsigned char mGreen;
      unsigned char mBlue;

      RGB(
         unsigned char red,
         unsigned char green,
         unsigned char blue
      )  :
         mRed(red), mGreen(green), mBlue(blue)
      {
      }
   };

#endif /* RGB_H_ */
