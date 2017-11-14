#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include <cvblobs/BlobResult.h>
//#include "BlobResult.h"

using namespace cv;  // The new C++ interface API is inside this namespace. Import it.
using namespace std;

std::string folderpath="/media/hoh-6/Datos/David/U/ai/database/DataBase_Letras/";
std::stringstream path;

FILE *fp;


CBlobResult blobs; 

int main(){


Mat imagen, imagenGris, imagenUmbral;

if((fp=fopen("datosEntrenamientoLetras.txt", "w+"))==NULL) { 
	printf("Cannot open file.\n"); 
	exit(1); 
} 

for(int l=0; l<5;l++){
	for(int k=1; k<=20; k++){
		std::stringstream path;
		path<<folderpath<<(char)(l+65)<<"_4"<<"_("<<k<<").jpg"; //i+1

		//load images
		imagen= imread(path.str().c_str());

		imagenUmbral.create(imagen.rows, imagen.cols, CV_8UC1);

		cvtColor(imagen, imagenGris, COLOR_BGR2GRAY);

		for(int j=0; j<imagenGris.rows; j++){
			uchar* data= imagenGris.ptr<uchar>(j);
			uchar* data1= imagenUmbral.ptr<uchar>(j);
			for(int i=0; i<imagenGris.cols; i++){
				if(data[i]>=100 && data[i]<=200){
					data1[i]=255;
				}
				else{
					data1[i]=0;
				}
			}
		}


		IplImage IplimagenUmbral = imagenUmbral;
		IplImage Iplimagen = imagen;

		blobs = CBlobResult( &IplimagenUmbral,NULL,255);

		CBlob *currentBlob;	

		blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER,  1900);

		int numBlobs=blobs.GetNumBlobs();

		if(numBlobs==1){
		for (int i = 0; i < blobs.GetNumBlobs(); i++ )
			{
				currentBlob = blobs.GetBlob(i);
				currentBlob->FillBlob(&Iplimagen, CV_RGB( 0, 0, 255));
				CBlobGetArea getArea;
				CBlobGetPerimeter getPerimetro;
				CBlobGetBreadth getBreadth;
				CBlobGetMaxXatMaxY getMaxXatMaxY;
				CBlobGetAreaElipseRatio getAEratio;
				float AEratio = getAEratio(*currentBlob);
				int maxXatMaxY = getMaxXatMaxY(*currentBlob);
				int breadth = getBreadth(*currentBlob);
				int Area=getArea(*currentBlob);
				int Perimetro=getPerimetro(*currentBlob);
				cout<<"Area"<<Area<<endl;
				cout<<"Perimetro"<<Perimetro<<endl;
				cout<<"Amplitud"<<breadth<<endl;
				cout<<"maxXatMaxY"<<maxXatMaxY<<endl;
				fprintf(fp,"%c, ",(char)(l+65)); 
				fprintf(fp,"%d, ",Area); 
				fprintf(fp,"%d, ",Perimetro);
				fprintf(fp,"%d, ",breadth);
				fprintf(fp,"%d, ",maxXatMaxY);
				fprintf(fp,"%f\n",AEratio);
			}
		}

		imshow("Original",imagen);

		imshow("Umbral",imagenUmbral);

		waitKey(10);
	}
}

fclose(fp);
getchar();

return 0;

}