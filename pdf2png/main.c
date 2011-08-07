//
//  main.c
//  pdf2png
//
//  Created by Ian Levesque on 8/7/11.
//  Copyright 2011 Ephemeron Industries. All rights reserved.
//

#import <ApplicationServices/ApplicationServices.h>

#define SIZE 480

int main (int argc, const char * argv[])
{
    if(argc >= 2) {
        CGPDFDocumentRef doc = CGPDFDocumentCreateWithProvider(CGDataProviderCreateWithFilename(argv[1]));
        size_t pages = CGPDFDocumentGetNumberOfPages(doc);
        
        printf("%lu pages\n", pages);
        
        for(size_t i = 1; i <= pages; i++) {
            char filename[1024];
            
            snprintf(filename, 1024, "%s.%03lu.png", argv[1], i);
            
            printf("writing file: %s\n", filename);
         
            CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
            CGContextRef context = CGBitmapContextCreate(NULL, SIZE, SIZE, 8, 4 * SIZE, colorSpace, kCGImageAlphaPremultipliedLast);
            CGColorSpaceRelease(colorSpace);
            
            CGContextDrawPDFDocument(context, CGRectMake(0, 0, SIZE, SIZE), doc, (int)i);
            
            CGImageRef image = CGBitmapContextCreateImage(context);
            
            CGImageDestinationRef dest = CGImageDestinationCreateWithURL(CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFStringCreateWithCString(kCFAllocatorDefault, filename, kCFStringEncodingASCII), kCFURLPOSIXPathStyle, false), kUTTypePNG, 1, NULL);
            
            CGImageDestinationAddImage(dest, image, NULL);
            
            CGImageDestinationFinalize(dest);
            
            CGImageRelease(image);
            CGContextRelease(context);
        }
        
        CGPDFDocumentRelease(doc);
    } else {
        printf("pdf2png [filename]\n");
        return 1;
    }
    
    return 0;
}

