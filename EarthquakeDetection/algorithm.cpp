#include "algorithm.h"

Algorithm::Algorithm()
{


}

void Algorithm::setParameters(float highPass, float lowPass, int longPoint, int shortWin, int staLtaTreshold, int winLength, double eew_a1, double eew_a2, double eew_a3, double eew_a4)
{
   f1 = highPass; f2 = lowPass; n1 = longPoint; ns = shortWin; thresh = staLtaTreshold; win_length = winLength;
   w1 = f1 / (fs/2);
   w2 = f2 / (fs/2);
   a1 = eew_a1; a2 = eew_a2; a3 = eew_a3; a4 = eew_a4;
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

double Algorithm::trapz(QVector<double> temp)
{
    double sum = 0;
        int SIZE = temp.length();
        for(int i=0; i<SIZE; i++) {
            if(i==0 || i== SIZE-1) {
               sum = sum + (temp.at(i)/2);;
            } else {
               sum = sum + temp.at(i);
            }
        }
        return sum;
}

double Algorithm::trapzWithPower2(QVector<double> temp)
{
    double sum = 0;
        int SIZE = temp.length();
        for(int i=0; i<temp.length(); i++) {
            if(i==0 || i== SIZE-1) {
               sum = sum + ((temp.at(i)*temp.at(i))/2);;
            } else {
               sum = sum + (temp.at(i)*temp.at(i));
            }
        }
        return sum;
}

float Algorithm::runAlgorithm(QVector<double> data, double *erthMagnitude)
{
    int k1;
    *erthMagnitude = 0;
    QVector<double> pstime;
    nt = data.length();
    QVector<double> sra;
    int temp = nt-ns;
    QVector<double> aseries = absVector(data);
    for(int i=0; i< temp; i++) {
        if((i+1)>n1) {
           double lta = meanQVector(splitQvector(aseries, i-n1, i) );
           double sta = meanQVector(splitQvector(aseries, i, i+ ns) );
           sra.append(sta/lta);
        } else {
           sra.append(0);
        }
    }
    temp = sra.length();
    QVector<double> k;
    for(int i=0; i< temp; i++) {
        if(sra[i] > thresh) {
            k.append((i+1));
            pstime.append((i+1)*dt);
        }
    }
    if(pstime.length() > 0) {
//        qDebug() << "pstime.length() > 0";
        k1 = k.first();
//        qDebug() << "k1:"<< k1 << " ((3/dt)+k1) :" << ((3/dt)+k1);
        QVector<double> acc_signal3 =  splitQvector(data, k1-1, ((3/dt)+k1)-1  ) ;
//        qDebug() << "acc_signal3.length():"<< acc_signal3.length() ;
        QVector<double> vel_signal3, dis_signal3;
        int pointsacc= acc_signal3.length();
        for(int i=0;i<(pointsacc-1);i++) {
         vel_signal3.append(dt*trapz( splitQvector(acc_signal3, i, i+1) ) );
        }
        double dmB = 0;
        for(int i=0;i<(pointsacc-2);i++) {
          double buffTemp = dt*trapz( splitQvector(vel_signal3, i, i+1) );
          dis_signal3.append(buffTemp);
          if(abs(buffTemp)> abs(dmB)) {dmB = buffTemp;}
        }
        double tavc = 2*M_PI*sqrt(trapzWithPower2(vel_signal3)/trapzWithPower2(dis_signal3));
        *erthMagnitude = tavc;
        if( ( a2< abs(dmB) ) && ( a1< abs(tavc) ) ) {
           return earthquake_Alarm;
        } else {
            return earthquake_no_Alarm;
        }
    }
    return no_earthquake;
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

QVector<double> Algorithm::absVector(QVector<double> temp)
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
    if(start > temp.length() ) {
        qDebug() << "splitQvector start > temp.length()";
        return tempBuffer;
    }
    int sizeVector = stop+1;
    if(stop >= temp.length() ) {
        qDebug() << "splitQvector stop >= temp.length()";
        sizeVector = temp.length();
    }

    for(int i =start; i<sizeVector;i++) {
        tempBuffer.append(temp[i]);
    }
    return tempBuffer;
}
