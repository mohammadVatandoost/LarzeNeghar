#include "algorithm.h"

Algorithm::Algorithm()
{
     qDebug() << "Algorithm()****";
    // 4th order butterworth
    a = { 1, -5.568408742, 13.56000484, -19.04102622, 17.01482891, -9.949832147, 3.707336739, -0.800529257, 0.077625871};
    b = { 0.018164321, 0, -0.072657282, 0, 0.108985924, 0, -0.072657282, 0, 0.018164321};
    qDebug() << "w1 :" << w1 << " , " << w2;
    butterWorthTest();


}

void Algorithm::setParameters(float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength)
{
   f1 = highPass; f2 = lowPass; n1 = longPoint; ns = shortWin; thresh = staLtaTreshold; win_length = winLength;
   w1 = f1 / (fs/2);
   w2 = f2 / (fs/2);
   // 4th order butterworth

   // filtering
}

void Algorithm::butterWorthTest()
{
//    qDebug() << "fs :" << fs << ", w1 :"<<w1<<", w2:"<<w2<<", filterOrder:"<< filterOrder << ", overallGain:"<<overallGain;
//    bool designedCorrectly = butterworth.loPass(fs,  // fs
//                                                    w1,    // freq1
//                                                    w2,      // freq2. N/A for lowpass
//                                                    filterOrder,
//                                                    coeffs,
//                                                    overallGain);
//    if(designedCorrectly) {qDebug() << "designedCorrectly";}
//   qDebug() << "coeffs.size() :"<< coeffs.size();
//   for(int i=0; i<coeffs.size(); i++) {
//       qDebug() << i << " : " << coeffs.at(i).a1 << ","<< coeffs.at(i).a2 << ","<< coeffs.at(i).a3 << ","<< coeffs.at(i).a4 << ","<< coeffs.at(i).b0 << ","<< coeffs.at(i).b1 << ","<< coeffs.at(i).b2 << ","<< coeffs.at(i).b3 << ","<< coeffs.at(i).b4 ;
//   }
}

void Algorithm::getButterWorthCoeffs()
{
//    bool designedCorrectly = butterworth.loPass(fs,  // fs
//                                                    w1,    // freq1
//                                                    w2,      // freq2. N/A for lowpass
//                                                    filterOrder,
//                                                    coeffs,
//                                                    overallGain);
//    if(designedCorrectly) {
//        qDebug() << "designedCorrectly";
//        a.append(coeffs.at(0).a1);a.append(coeffs.at(0).a2);a.append(coeffs.at(0).a3);a.append(coeffs.at(0).a4);
//        a.append(coeffs.at(0).b0);a.append(coeffs.at(0).b1);a.append(coeffs.at(0).b2);a.append(coeffs.at(0).b3);a.append(coeffs.at(0).b4);

//        b.append(coeffs.at(1).a1);b.append(coeffs.at(1).a2);b.append(coeffs.at(1).a3);b.append(coeffs.at(1).a4);
//        b.append(coeffs.at(1).b0);b.append(coeffs.at(1).b1);b.append(coeffs.at(1).b2);b.append(coeffs.at(1).b3);b.append(coeffs.at(1).b4);
    //    }
}

float Algorithm::runAlgorithm(QVector<double> data)
{
    nt = data.length();
    QVector<float> sra;
    int temp = nt-ns;
    for(int i=0; i< temp; i++) {
        if(i>n1) {
           double lta = meanQVector(splitQvector(data, i-n1, i) );
           double sta = meanQVector(splitQvector(data, i, i+ ns) );
           sra.append(sta/lta);
        } else {
           sra.append(0);
        }
    }
    temp = sra.length();
    for(int i=0; i< temp; i++) {
        if(sra[i] > thresh) {
            return i*dt;
        }
    }
    return 0;
}

QVector<double> Algorithm::bandPassFilter(QVector<double> data)
{
    /*

    Input:

    f1, the lowest frequency to be included, in Hz

    f2, the highest frequency to be included, in Hz

    f_samp, sampling frequency of the audio signal to be filtered, in Hz

    N, the order of the filter; assume N is odd

    Output:

    h, a bandpass FIR filter in the form of an N-element array */

//    {

    //Normalize f_c and ω _c so that pi is equal to the Nyquist angular frequency

//      int f1_c = f1/fs;
//      int f2_c = f2/fs;

//      int ω1_c = 2*pi*f1_c;

//      int ω2_c = 2*pi*f2_c;
//      int N = 4 ; // filter order
//      int middle = N/2;    /*Integer division, dropping remainder*/

//      for i = −N/2 to N/2

//       if (i == 0) h(middle) = 2*(f2_c – f1_c)

//       else

//      h(i + middle) = sin(ω2_c*i)/(pi*i) – sin(ω1_c*i)/(pi*i)

//     }
}

QVector<int> Algorithm::absVector(QVector<int> temp)
{
    int sizeVector = temp.size();
    for(int i=0;i<sizeVector; i++) {
        temp[i] = abs(temp[i]);
    }
    return temp;
}

QVector<int> Algorithm::zeros(int size)
{
    QVector<int> temp;
    for(int i=0; i<size;i++) {
        temp.append(0);
    }
    return temp;
}

double Algorithm::meanQVector(QVector<double> temp)
{
    double sum = 0;
    int sizeVector = temp.size();
    for(int i =0; i<sizeVector;i++) {
        sum = sum + temp[i];
    }
    return sum/sizeVector;
}

QVector<double> Algorithm::splitQvector(QVector<double> temp, int start, int stop)
{
    QVector<double> tempBuffer;
    int sizeVector = stop+1;
    for(int i =start; i<sizeVector;i++) {
        tempBuffer.append(temp[i]);
    }
    return tempBuffer;
}
