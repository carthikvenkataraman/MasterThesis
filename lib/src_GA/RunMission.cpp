#include "Combination.h"


void Combination::GetInstantaneousCycleData(double &instantaneousGradient , double &instantaneousTargetSpeed, double totalDistanceCovered )
{
	int selected_counter;
	double selected_ratio;

	for(int i= 1; i <=londitudinalPosition.size();i++)
	{
		if(totalDistanceCovered < londitudinalPosition[i])
		{
			selected_counter = i-1;
			selected_ratio = (totalDistanceCovered - londitudinalPosition[i-1])/ (londitudinalPosition[i]-londitudinalPosition[i-1]);
			break;
		}
	}
		
	instantaneousGradient = londitudinalPosition[selected_counter] ;
	instantaneousTargetSpeed = targetSpeed[selected_counter] + selected_ratio *  (targetSpeed[selected_counter+1]-targetSpeed[selected_counter]);
}	

double* Combination::GetTorqueDistributionRatio(double axleActive[],double axleCount) 
{
	int i;
	double denominator=0.00;
	

	if(torqueResistive < 0)
	{
		for(i=0;i<axleCount;i++)
		{
			if(axlesActive[i]==1)
			{
				denominator += (1-chargeLevelRatio[i]);
			}
		}
		for(i=0;i<axleCount;i++)
		{
			if(axlesActive[i]==1)
			{
				chargeLevelRatio[i] = (1-chargeLevelRatio[i])/ denominator;
			}
		} 
	}
	else
	{
		for(i=0;i<axleCount;i++)
		{
			if(axlesActive[i]==1)
			{
				denominator += chargeLevelRatio[i];
			}
		}
		for(i=0;i<axleCount;i++)
		{
			if(axlesActive[i]==1)
			{
				chargeLevelRatio[i] = chargeLevelRatio[i]/ denominator;
			}
		} 
	}

	return chargeLevelRatio;
}

void Combination::DistributeElectricTorque(double &torqueToDistributeAmongAxles) 
{
	int sign;
	sign = torqueToDistributeAmongAxles / (std::abs(torqueToDistributeAmongAxles));
	torqueToDistributeAmongAxles *=sign;

	while((std::max(axlesActive))||(torqueToDistributeAmongAxles))
	{
		for(int i =0;i<axleCount;i++)
		{
			if(axleActive[i]==1)
			{
				torqueForAxle[i] = std::min((torqueForAxle[i] +(chargeLevelRatio[i] * torqueToDistributeAmongAxles)), machineMaximumTorque[i] );
				torqueExtra += std::max((chargeLevelRatio[i] * torqueToDistributeAmongAxles) - machineMaximumTorque[i] , 0);
				axleActive[i]=(torqueExtra > 0)?0:1;
			}
			else
			{
				torqueForAxle[i]=0;
			}
		}
		torqueToDistributeAmongAxles = torqueExtra;
		*chargeLevelRatio = GetTorqueDistributionRatio(axlesActive, axleCount);
	}
	for(int i =0;i<axleCount;i++)
	{
		torqueForAxle[i] *= sign;
	}
	torqueToDistributeAmongAxles *=sign;
}


double Combination::TorqueAssignment(double torqueResistive, double torqueAccelerationPredicted, double wheelSpeed)
{
	double torqueToDistributeAmongAxles=0.0, torqueExtra=0.0, torqueforAxle[8],totalMaximumElectricTorque=0.0, powerForAxles[8],engineTorque=0.0 ;
	int i,j,axleCount=0,axlesActive[8],axleFlag=0;
	
	/*
	Not sure where to mention this but the plan is that the gear selection is done within contructors / local functions of the
	machine and engine class. The purpose would be to simply pass wheel speed to the said function and the instantaneous maximum torque 
	for both will be set within the function. The gear selection itself is irrelevent outside the class, only the instantaneous max torque 
	and optimum torque datawill be used. After torque assignment the gear data will be needed once again but only within the local functions 
	to compute fuel consumption. 
	*/

	*chargeLevelRatio = GetTorqueDistributionRatio(axlesActive, axleCount);

	if(torqueResistive + torqueAccelerationPredicted < 0)
	{
		torqueToDistributeAmongAxles = torqueResistive+ torqueAccelerationPredicted;  //		ATTENTION : all the logic assumes negative torque motor performance mirrors the positive
		DistributeElectricTorque(torqueToDistributeAmongAxles);
		engineTorque = 0.0; //engine torque
		torqueAcceleration = torqueToDistributeAmongAxles;
		return torqueAcceleration;
	}   
	else
	{	
		if((totalMaximumElectricTorque+engineMaximumTorque)<=(torqueResistive+torqueAccelerationPredicted))
		{
			if((totalMaximumElectricTorque+engineMaximumTorque)>=(torqueResistive-engineOptimumTorque))
			{
				engineTorque = engineOptimumTorque;
				//DistributeElectricTorque(torqueResistive-engineOptimumTorque); CHECK! - to account for effect of SoC level within allowable range
				for(i=0;i<axleCount;i++)
				{
					torqueForAxle[i] = (axleActive[i]==1)?machineMaximumTorque[i]:0;//or ?chargeDivisionRatio*machineMaximumTorque[i]:0
					torqueAcceleration += torqueForAxle[i];
				}
				torqueAcceleration -= torqueResistive;
			}
			else
			{
				engineTorque = engineMaximumTorque;
				for(i=0;i<axleCount;i++)
				{
					torqueForAxle[i] = (axleActive[i]==1)?machineMaximumTorque[i]:0;
					torqueAcceleration += torqueForAxle[i];
				}
				torqueAcceleration -= torqueResistive;
			}
		}
		else
		{
			if((totalMaximumElectricTorque+engineOptimumTorque)<=(torqueResistive+torqueAccelerationPredicted))
			{
				engineTorque=engineMaximumTorque;
				torqueAcceleration = torqueAccelerationPredicted;
				torqueToDistributeAmongAxles = (torqueResistive+torqueAcceleration)-engineMaximumTorque;
				DistributeElectricTorque(torqueToDistributeAmongAxles);
			}
			else
			{	
				engineTorque=engineOptimumTorque;
				torqueToDistributeAmongAxles = (torqueResistive+torqueAcceleration)-engineOptimumTorque;
				torqueAcceleration = torqueAccelerationPredicted;
				DistributeElectricTorque(torqueToDistributeAmongAxles);
			}
		}
	}

	torqueForAxle[1]=0.5*engineTorque;
	torqueForAxle[2]=0.5*engineTorque
	
	for(i=0;i<axleCount;i++)
	{
		powerForAxle[i] = torqueForAxle[i]*wheelSpeed* (2*3.14/60)*timeStep;
	}

	UnitsinCombination[0]->SetMachineOperatingEfficency(engineTorque,wheelSpeed); // sets the bsfc value which in the case of 1st unit is stored in machineOperatingEfficiency
	UnitsinCombination[0]->UpdateFuelDemand(powerForAxle[1]+powerForAxle[2]);

	for( j=1;j<UnitsinCombination.size();j++)
	{
		for( i=0;i<UnitsinCombination[j]->numberOfAxles;i++)
		{
			if (axlesActive[axleFlag]==1)
			{
				UnitsinCombination[j]->axlesInUnit[i]->axleTractiveForce= torqueForAxle[axleFlag];
				UnitsinCombination[j]->axlesInUnit[i]->SetMachineOperatingEfficency(torqueForAxle[axleFlag],wheelSpeed);
				UnitsinCombination[j]->UpdateChargeDemand(powerForAxle[i],i);
			}
		}
	}

}

std::vector<double> Combination::RunMission()
{
	//Variables:
	vector<double> missionResults;
	double dragForce , rollingForce, slopeForce, wheelSpeed, timeStep =1.00,totalDistanceCovered=0  ;  
	double initialFuelLevel = UnitsInCombination[0]->bufferInUnit->instantaneousBufferLevel;
	double finalFuelLevel;
	double totalFuelConsumed;
	
	while(instantaneousLongitudinalPosition <= londitudinalPosition.back())
	{
		GetInstantaneousCycleData(&instantaneousGradient , &instantaneousTargetSpeed, totalDistanceCovered);

		// Total Resistive Forces: 
		dragForce=0.5*Cd*A*rho*pow(instantaneousSpeed,2);
		rollingForce= Crr*Total_mass*g;
		slopeForce= Total_mass*g*sin(instantaneousGradient);
		roadResistiveForce=(DragForce+RollingForce+SlopeForce);
		torqueResistive = roadResistiveForce * wheelradius ;

		accelerationPredicted = (instantaneousTargetSpeed - instanataneousSpeed ) / timeStep;
		torqueAccelerationPredicted = grossCombinationWeight * accelerationPredicted * wheelradius ; 
		wheelSpeed=instantaneousSpeed*60/(2*3.14*wheelradius);

		gear = GearSelection(wheelSpeed);

		torqueAcceleration = TorqueAssignment(torqueResistive,torqueAccelerationPredicted, wheelSpeed);

		instantaneousAcceleration = torqueAcceleration / (grossCombinationWeight * wheelradius )
		totalDistanceCovered += instantaneousSpeed*timeStep + (0.5* instantaneousAcceleration*timeStep*timeStep);
		instantaneousSpeed += instantaneousAcceleration * timeStep ;
	}

	finalFuelLevel = UnitsInCombination[0]->bufferInUnit->instantaneousBufferLevel;
	totalFuelConsumed = initialFuelLevel - finalFuelLevel ;
}
