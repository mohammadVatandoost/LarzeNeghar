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
    qDebug() << "fs :" << fs << ", w1 :"<<w1<<", w2:"<<w2<<", filterOrder:"<< filterOrder << ", overallGain:"<<overallGain;
    bool designedCorrectly = butterworth.loPass(fs,  // fs
                                                    w1,    // freq1
                                                    w2,      // freq2. N/A for lowpass
                                                    filterOrder,
                                                    coeffs,
                                                    overallGain);
    if(designedCorrectly) {qDebug() << "designedCorrectly";}
   qDebug() << "coeffs.size() :"<< coeffs.size();
   for(int i=0; i<coeffs.size(); i++) {
       qDebug() << i << " : " << coeffs.at(i).a1 << ","<< coeffs.at(i).a2 << ","<< coeffs.at(i).a3 << ","<< coeffs.at(i).a4 << ","<< coeffs.at(i).b0 << ","<< coeffs.at(i).b1 << ","<< coeffs.at(i).b2 << ","<< coeffs.at(i).b3 << ","<< coeffs.at(i).b4 ;
   }
}

void Algorithm::getButterWorthCoeffs()
{
    bool designedCorrectly = butterworth.loPass(fs,  // fs
                                                    w1,    // freq1
                                                    w2,      // freq2. N/A for lowpass
                                                    filterOrder,
                                                    coeffs,
                                                    overallGain);
    if(designedCorrectly) {
        qDebug() << "designedCorrectly";
        a.append(coeffs.at(0).a1);a.append(coeffs.at(0).a2);a.append(coeffs.at(0).a3);a.append(coeffs.at(0).a4);
        a.append(coeffs.at(0).b0);a.append(coeffs.at(0).b1);a.append(coeffs.at(0).b2);a.append(coeffs.at(0).b3);a.append(coeffs.at(0).b4);

        b.append(coeffs.at(1).a1);b.append(coeffs.at(1).a2);b.append(coeffs.at(1).a3);b.append(coeffs.at(1).a4);
        b.append(coeffs.at(1).b0);b.append(coeffs.at(1).b1);b.append(coeffs.at(1).b2);b.append(coeffs.at(1).b3);b.append(coeffs.at(1).b4);
    }
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

float Algorithm::meanQVector(QVector<int> temp)
{
    int sum = 0;
    int sizeVector = temp.size();
    for(int i =0; i<sizeVector;i++) {
        sum = sum + temp[i];
    }
    return sum/sizeVector;
}

QVector<int> Algorithm::splitQvector(QVector<int> temp, int start, int stop)
{
    QVector<int> tempBuffer;
    int sizeVector = stop+1;
    for(int i =start; i<sizeVector;i++) {
        tempBuffer.append(temp[i]);
    }
    return tempBuffer;
}
