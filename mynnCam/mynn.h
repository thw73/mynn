#ifndef MYNN_H
#define MYNN_H


#include <vector>

#include <Eigen/Dense>


class neuralNetwork 
{

  public:
    neuralNetwork(int inputnodes, int hiddennodes, int outputnodes, float learningrate);

    void train(Eigen::Matrix<double,1,Eigen::Dynamic> inputs_list, Eigen::Matrix<double,1,Eigen::Dynamic> targets_list);
    Eigen::Matrix<double,Eigen::Dynamic, 1> query( Eigen::Matrix<double,1,Eigen::Dynamic> inputs_list);
	
	
	int inodes;
    int hnodes;
    int onodes;
	
    Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> wih;
    Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic> who;

    //typedef Matrix<double, Dynamic, Dynamic> MatrixXd;

	
	float lr;
	
};
#endif
