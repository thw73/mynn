#ifndef MYNN_H
#define MYNN_H


#include <QString>

#include <Eigen/Dense>



class neuralNetwork 
{

  public:
	neuralNetwork(int inputnodes, int hiddennodes, int outputnodes, float learningrate);
	
    void train(Eigen::Matrix<double,1,Eigen::Dynamic> inputs_list, Eigen::Matrix<double,1,Eigen::Dynamic> targets_list);
    Eigen::Matrix<double,Eigen::Dynamic, 1> query( Eigen::Matrix<double,1,Eigen::Dynamic> inputs_list);
    void loadWeights(QString wih_data_file,QString who_data_file);
    void saveWeights(QString pathWih,QString pathWoh);
    int loadAndTrain(QString TrainDataFileName);
    int calcScorecard(QString TestDataFileName);

	
    int inodes;
    int hnodes;
    int onodes;
	
	Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> wih;
	Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> who;

	float lr;
	
};
#endif
