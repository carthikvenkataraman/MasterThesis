#include "Axle.h"

typedef std::vector<int> Genes;
typedef std::vector<Genes> UnitGene;
typedef std::vector<UnitGene> Individual;

Axle::Axle(int currentAxleIndex, int currentAxlePropulsionFlag, std::shared_ptr<Machine> currentMachineForAxle) {
	//std::cout<<"Axle creation begun"<<std::endl;
    axleIndex = currentAxleIndex;
    axlePropulsionFlag = currentAxlePropulsionFlag;
    machineForAxle = currentMachineForAxle;
}

int Axle::GetAxleIndex() {
	return axleIndex;
}

std::shared_ptr<Machine> Axle::ReturnMachineForAxle() {
	if(machineForAxle!=NULL) {
		return machineForAxle;
	};
}

int Axle::GetAxlePropulsionFlag() {
	return axlePropulsionFlag;
}

void Axle::DefineAxleParameters(double currentAxleLoad, double currentAxleTireRadius, 
												double axleCoefficientOfFriction, double finalDriveRatio) {
	axleLoad = currentAxleLoad;
	axleTireRadius = currentAxleTireRadius;
	coefficientOfFriction = axleCoefficientOfFriction;
	axleFinalDriveRatio = finalDriveRatio;
}

double Axle::GetAxleCoefficientOfFriction() {
	return coefficientOfFriction;
}
 
double Axle::GetAxleLoad() {
	return axleLoad;
}

void Axle::UpdateAxleWheelSpeed(double instantaneousSpeed) {
	axleTireRPM = instantaneousSpeed*60/(2*3.14159*axleTireRadius);	
	RPMToAxleDifferential = axleTireRPM*axleFinalDriveRatio;
	//std::cout<<"axleTireRPM "<<axleTireRPM<<std::endl<<"RPMToAxleDifferential "<<RPMToAxleDifferential<<std::endl;
}

double Axle::GetTractiveForceAtWheels(double torqueAtDifferential) {
	//std::cout<<"axleFinalDriveRatio "<<axleFinalDriveRatio<<std::endl<<"axleTireRadius "<<axleTireRadius<<" m"<<std::endl;
	tractiveForceAtWheels = torqueAtDifferential * axleFinalDriveRatio / axleTireRadius;
	//std::cout<<"tractiveForceAtWheels corresponding to "<<torqueAtDifferential<<" Nm of torque at the differential is "<<tractiveForceAtWheels<<" N"<<std::endl;
	return tractiveForceAtWheels;
}

void Axle::GetTorqueToAxleDifferential(double tractiveForceAtWheels) {
	//std::cout<<"Calculating torque at axle differential given tractive force of "<<tractiveForceAtWheels<<" N at wheels"<<std::endl;
	if(tractiveForceAtWheels>gripLimitedTraction) {
		//std::cout<<"tractiveForceAtWheels exceeds gripLimitedTraction "<<gripLimitedTraction<<" N"<<std::endl;
		tractiveForceAtWheels=gripLimitedTraction;
	}
	axleTractiveForce = tractiveForceAtWheels;
	//std::cout<<"axleTractiveForce "<<axleTractiveForce<<" N"<<std::endl;
	double driveShaftTorque = tractiveForceAtWheels*axleTireRadius;
	//std::cout<<"driveShaftTorque "<<driveShaftTorque<<" Nm"<<std::endl;
	torqueToAxleDifferential = driveShaftTorque/axleFinalDriveRatio;
	//std::cout<<"Torque to axle differential "<<torqueToAxleDifferential<<std::endl;
}

Axle::~Axle() {
}