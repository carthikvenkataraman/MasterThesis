//#include<iostream.h>
#include<conio.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<math.h>
#include<vector>

using namespace std;

//interpolation function:
float interpolate(float engine_speed, float Set_speed[], float Maximum_torque[])
{
	float selected_ratio,selected_torque,SelectedMaximumTorque;
	int selected_counter;
	//Interpolation:
	for(int i= 1; i <=13;i++)
	{
		if(engine_speed < Set_speed[i])
		{
			selected_counter = i-1;
			selected_ratio = (engine_speed - Set_speed[i-1])/ (Set_speed[i]-Set_speed[i-1]);
			break;
		}
	}

	SelectedMaximumTorque = Maximum_torque[selected_counter] + selected_ratio *  (Maximum_torque[selected_counter+1]-Maximum_torque[selected_counter]);
	return SelectedMaximumTorque;
}

//read fucntion
void read_file(vector<float> &cycle_aim_velocity, vector<float> &cycle_grade, vector<float> &cycle_distance)
{
	ifstream cycle_data;
	//open the text file
    cycle_data.open("test cycle - cruise - dist.txt");
	//cycle_data.open("test slope - cruise - time.txt");
	//cycle_data.open("test gradeability - cruise.txt");
	cout<<"\n opened file - begin reading into vector...";
	
	float velocity,slope,distance;
	
	
	if (!cycle_data ) 
	{
        cout<< "Unable to open file";
		_getch();
        exit(1); // terminate with error
    }

	while (!cycle_data.eof())
	{
		cycle_data>>velocity>>distance>>slope;
		cycle_aim_velocity.push_back(velocity);
		cycle_distance.push_back(distance);
		cycle_grade.push_back(slope);
		
	}
	
	cycle_data.close();
	cout<<"\n read complete- file closed";
}

void write_file(vector<float> Instantaneous_power, vector<float> velocity_actual, vector<float> acceleration_actual, vector<float> Selected_gear, vector<float> Instantaneous_FuelConsumption, vector<float> cycle_time, vector<float> Maximum_ResistiveForce, vector<float> Selected_engine_speed, vector<float> Demand_torque, vector<float> Road_ResistiveForce, vector<float> distance_log,vector<float> slope_log)
{
	
	ofstream results("results - cruise.txt",ios::trunc);

	int counter;
	for(counter=0;counter<acceleration_actual.size();counter++)
	{
		results<<Instantaneous_power[counter]<<" "<<velocity_actual[counter]<<" "<<acceleration_actual[counter]<<" "<<Selected_gear[counter]<<" "<<Instantaneous_FuelConsumption[counter]<<" "<<cycle_time[counter]<<" "<<Maximum_ResistiveForce[counter]<<" "<<Road_ResistiveForce[counter]<<" "<<Selected_engine_speed[counter]<<" "<<Demand_torque[counter]<<" "<<distance_log[counter]<<" "<<slope_log[counter]<<endl;
	}
	results.close();
}

void write_input(vector<float> velocity ,vector<float> grade,vector<float> distance)
{
	ofstream temp_log("temp log - cruise.txt",ios::trunc);

	int counter;
	for(counter=0;counter<velocity.size();counter++)
	{
		temp_log<<velocity[counter]<<" "<<grade[counter]<<" "<<distance[counter]<<endl;
	}
	temp_log.close();
}


void main()
{
	//variable declaration : 
	float interpolation(float, float [] , float []);
	void read_file(vector<float> &, vector<float> &, vector<float> &);
	void write_file(vector<float> , vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>,vector<float>);
	void write_input(vector<float> ,vector<float> ,vector<float> );
	float Set_speed[]={700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000};
	float Maximum_torque[]={1840,2000,2400,3300,3550,3550,3550,3550,3400,3290,3095,2924,2737,1906};
	//float Maximum_torque[]={3000,3100,3000,3200,3200,3200,3300,3300,3400,3500,3600,3500,3400,3300};
	float Set_torque[37]= {0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500,2600,2700,2800,2900,3000,3100,3200,3300,3400,3500,3600};	//[Nm]
	float BrakeSpecificFuelConsumption[37][14]=	{	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0},
							{ 397.40,416.65,424.91,448.45,458.25,481.80,502.08,522.19,548.84,575.14,601.43,630.48,661.94,693.57},
							{ 287.57,297.71,297.19,306.82,312.32,323.84,332.69,340.85,357.35,369.56,381.85,395.77,412.87,430.49},
							{ 250.33,253.65,256.46,260.92,265.16,271.01,278.40,283.44,295.88,301.78,308.82,318.68,330.25,343.49},
							{ 231.75,230.93,236.09,237.98,241.72,244.55,251.34,255.21,265.14,267.93,272.36,280.56,289.03,299.99},
							{ 220.74,220.39,223.59,225.38,227.48,230.05,235.55,238.34,245.90,247.41,251.13,258.93,266.25,275.43},
							{ 213.43,213.54,215.29,217.07,217.95,220.42,225.12,227.15,233.05,233.74,236.98,244.42,251.04,259.06},
							{ 209.70,209.38,210.19,211.05,211.10,218.32,220.02,220.06,227.16,225.59,228.14,233.15,238.48,245.82},
							{ 207.00,206.24,206.37,206.52,205.92,217.18,216.19,214.73,222.83,219.48,221.54,224.70,229.06,235.87},
							{ 204.89,203.92,203.65,204.24,203.19,212.72,211.73,210.28,218.11,215.87,217.97,219.21,223.51,229.58},
							{ 203.24,202.04,201.66,202.55,200.95,209.05,207.57,206.71,214.33,213.07,215.20,214.86,219.07,224.73},
							{ 202.62,201.14,200.59,200.11,198.53,203.50,203.78,205.08,210.02,209.28,212.30,211.61,215.58,221.78},
							{ 202.11,200.41,199.70,198.01,196.52,199.13,200.59,203.63,206.41,206.15,209.76,208.90,212.70,219.43},
							{ 202.75,199.89,198.68,196.72,194.96,197.18,197.99,200.66,203.21,203.81,206.91,206.34,210.40,217.34},
							{ 203.43,199.89,197.68,195.61,193.62,195.51,195.82,198.04,200.46,201.81,204.51,204.15,208.45,215.55},
							{ 205.50,201.09,197.60,194.83,193.28,194.10,194.62,195.26,200.16,200.00,202.64,202.35,207.18,213.51},
							{ 207.33,202.13,197.53,194.21,193.06,192.86,193.57,192.84,199.97,198.43,201.30,200.76,206.09,211.68},
							{ 208.78,202.91,197.44,194.28,191.78,192.37,192.91,191.51,197.28,197.47,200.84,200.16,205.53,210.89},
							{ 210.08,203.61,197.45,194.36,190.60,191.96,192.30,190.62,194.88,196.62,200.37,199.63,205.00,210.21},
							{ 211.24,204.29,197.76,193.68,190.41,193.43,191.46,190.86,194.14,195.99,199.68,199.13,204.40,205.94},
							{ 212.29,204.92,198.11,193.06,190.24,194.25,190.72,191.07,193.46,195.38,199.05,198.68,203.90,202.10},
							{ 213.23,205.47,198.89,192.87,190.03,193.17,190.41,190.85,192.77,194.72,198.32,198.21,203.64,198.62},
							{ 214.09,205.98,199.60,192.90,189.70,192.19,190.14,190.58,192.23,194.13,197.66,197.77,203.40,195.46},
							{ 214.87,206.44,200.25,192.95,189.06,189.91,189.70,189.26,192.21,193.78,197.57,197.76,202.84,192.57},
							{ 215.60,206.87,200.84,193.02,188.54,187.79,189.31,188.04,192.21,193.47,197.49,197.79,202.38,189.93},
							{ 216.25,207.25,201.39,192.61,189.16,187.41,188.99,187.45,192.62,193.88,197.71,198.10,202.61,187.49},
							{ 216.87,207.62,201.89,192.20,189.72,187.28,188.71,186.91,192.99,194.27,197.95,198.38,202.82,185.25},
							{ 217.43,207.95,202.36,192.65,189.48,187.32,188.70,187.22,192.56,194.81,198.43,199.26,203.80,183.17},
							{ 217.96,208.27,202.80,193.06,189.12,187.50,188.74,187.72,192.22,195.28,198.86,200.22,204.72,181.24},
							{ 218.45,208.55,203.19,192.63,188.92,188.18,188.94,188.84,192.61,195.36,199.03,201.68,205.57,179.44},
							{ 218.90,208.82,203.57,192.17,188.74,188.80,189.09,189.88,192.99,195.43,199.19,203.04,206.36,177.77},
							{ 219.33,209.08,203.92,192.32,189.20,188.95,189.15,190.18,193.60,196.05,200.27,204.32,207.10,176.20},
							{ 219.73,209.31,204.26,192.46,189.72,189.09,189.20,190.83,194.18,196.83,201.29,205.52,207.80,174.72},
							{ 220.10,209.54,204.57,192.59,190.04,189.21,189.17,193.50,195.52,198.76,202.24,206.64,208.45,173.34},
							{ 220.46,209.74,204.86,192.72,190.36,189.38,189.90,195.78,196.79,200.59,203.13,207.70,209.07,172.04},
							{ 220.79,209.94,205.14,192.83,190.74,190.14,192.02,196.76,197.98,202.31,203.98,208.70,209.64,170.81},
							{ 221.10,210.13,205.40,192.94,191.10,190.84,194.03,197.68,199.12,203.94,204.77,209.64,210.19,169.66}  }; //[g/kWh]
	

	
	float grade,aim_velocity,gradeability_scale,delta_time=0,Total_time=0,line=0,vel_old=0,SelectedMaximumTorque,scale=0.001,value=0;
	float t1=0,t2=0,acceleration_max=0,distance=0,d,total_distance=0,c,acceleration_predicted=0;
	float ratios[12]={11.73,9.21,7.09,5.57,4.35,3.41,2.70,2.12,1.63,1.28,1.00,0.78}, final_drive =3.00;
	int mode = 0,speed_counter,torque_counter,flag=1,gear,flip_flag=0,flag_limit=0,deceleration_flag=0,j,Total_ResistiveForce,maxlimit_error,gear_drop,select=0,gear_new=0,selected_counter;
	float DragForce=0.00,RollingForce=0.00,SlopeForce=0.00,wheel_speed,Total_FuelConsumption=0,bsf,w_opt[14],selected_ratio,selected_torque,engine_speed;
	float Power_demand,AccelerationForce=0.00,Tidle=2700,w_idle=700,Tdem_old=0,minimum,time_step=1.00;
	float Maximum_power=559274.904;  //750HP
	float Selected_power=559274.904;  //520HP - 387763.933*0.4
	float engine_speed_lowergear,engine_speed_highergear, engine_torque_lowergear,engine_torque_highergear;
	vector<float> cycle_aim_velocity,cycle_time,cycle_grade,cycle_distance,Road_ResistiveForce,Maximum_ResistiveForce,velocity_actual,Instantaneous_power,Demand_torque,Selected_engine_speed,Selected_gear,Instantaneous_FuelConsumption,acceleration_actual, distance_log,slope_log;


	//Truck Data:
	
	long int mtractor=21000;						//Mass of tractor [Kg]
	long int mtrailer=5000;							//Mass of trailer[Kg]
	long int mload=27000;							//Mass of load carried (maximum of combined 50 tonnes for DUO^2)[Kg]
	long int mdolly=2000;							//Mass of dolly[Kg]
	float g=9.82;									//gravity constant[m/s^2] 
	float wheelradius=0.4;							//dolly wheel radius[m]
	int mextra=20000;
	int Total_mass = mtractor+(2*mtrailer)+mload+mdolly+mextra;	//total weight
	float Cd=0.6; 
	float A=9.9;							//[m^2]Frontal area of volvo FH
	float rho=1.2;
	float Crr=0.005;						//large truck tires data from Volvo
	float my=0.77;							//friction coefficient 0.2 for snow 0.4 for wet and 0.8 for dry

	read_file(cycle_aim_velocity, cycle_grade, cycle_distance);
	write_input(cycle_aim_velocity, cycle_grade, cycle_distance);
    
	cout<<"\n loop successfully started : ";
	
	
	
	velocity_actual.push_back(0);

	while (total_distance<=cycle_distance.back()-1)
    {
		//initialisation:
		gradeability_scale = 0.15;
		gear_drop=0;
		maxlimit_error = 0; 

		velocity_actual.push_back(0);
		acceleration_actual.push_back(0);
		Road_ResistiveForce.push_back(0);
		Selected_engine_speed.push_back(0);
		Maximum_ResistiveForce.push_back(0);
		Demand_torque.push_back(0);
		Instantaneous_power.push_back(0);
		Instantaneous_FuelConsumption.push_back(0);
		Selected_gear.push_back(0);
		cycle_time.push_back(0);
		distance_log.push_back(total_distance);
		slope_log.push_back(0);


		//slope information 
		for(int i= 1; i <=cycle_distance.size();i++)
		{
			if(total_distance < cycle_distance[i])
			{
				selected_counter = i-1;
				selected_ratio = (total_distance - cycle_distance[i-1])/ (cycle_distance[i]-cycle_distance[i-1]);
				break;
			}
		}
		
		grade = cycle_grade[selected_counter] ;
		aim_velocity = cycle_aim_velocity[selected_counter] + selected_ratio *  (cycle_aim_velocity[selected_counter+1]-cycle_aim_velocity[selected_counter]);
		//+ selected_ratio *  (cycle_grade[selected_counter+1]-cycle_grade[selected_counter]);
		slope_log[line]=grade;
		
		//Power demand calculation

		DragForce=0.5*Cd*A*rho*pow(velocity_actual[line],2);
		RollingForce= Crr*Total_mass*g;
		SlopeForce= Total_mass*g*sin(grade);
		Road_ResistiveForce[line]=(DragForce+RollingForce+SlopeForce);

		//Calculating desired acceleration using :  v=u+at (t= 1s)
		acceleration_predicted = (aim_velocity-velocity_actual[line])/time_step;
		
		
		wheel_speed=velocity_actual[line]*60/(2*3.14*wheelradius);
		
	
		//Gear selection - running lowest possible gear for fastest acceleration
		
		for(gear=11; gear>=0;gear--)
		{
			minimum = 2000 / (ratios[gear]*final_drive);
			if(wheel_speed <= minimum)
			{
				continue;
			}
			else 
			{
				break;
			}
		}

		gear++;
		//end gear selection
		
		Selected_gear[line]=gear+1;
		engine_speed= wheel_speed*ratios[gear]*final_drive;
		engine_speed = (engine_speed<700)?700:engine_speed;
		Selected_engine_speed[line]=engine_speed;
		
		//Maximum Torque:
		SelectedMaximumTorque = interpolate(engine_speed, Set_speed,  Maximum_torque);
		Maximum_ResistiveForce[line] = ((SelectedMaximumTorque*Selected_power/Maximum_power)*(ratios[gear]*final_drive))/wheelradius;
		
		//F=ma
		
		acceleration_max= (Maximum_ResistiveForce[line]-RollingForce-DragForce-SlopeForce)/Total_mass;
		acceleration_actual[line] = std::min(acceleration_predicted, acceleration_max);
		if(acceleration_actual[line] < -1)
		{
				deceleration_flag++;
				acceleration_actual[line] = -1;
		}

		
			

		// Intantaneous Power:
		AccelerationForce=Total_mass*acceleration_actual[line];
		Total_ResistiveForce=DragForce+RollingForce+SlopeForce+AccelerationForce;
		Demand_torque[line]=Total_ResistiveForce*wheelradius;
		Demand_torque[line] = min(Demand_torque[line]/(ratios[gear]*final_drive),SelectedMaximumTorque);
		Instantaneous_power[line]=Demand_torque[line]*engine_speed*2*3.14/60;
				
		//torque association:
		if(Demand_torque[line]>0)
		{	
			//Demand_torque=min(Maximum_torque[speed_counter],Demand_torque);
			for(torque_counter=0;torque_counter<37;torque_counter++)
			{
				if(Demand_torque[line]<Set_torque[torque_counter])
				{
					break;
				}
			}
		
			if(torque_counter)
			{
				torque_counter--;
			}
		}
		//speed association 
		minimum = abs(engine_speed - Set_speed[0]);
		for(speed_counter=1;speed_counter<14;speed_counter++)
		{
			
			if(abs(engine_speed - Set_speed[speed_counter])<minimum)
			{
				minimum = abs(engine_speed - Set_speed[speed_counter]);

			}
			else 
			{
				break;
			}
		}

		if(speed_counter)
		{
			speed_counter--;
		}
		
		//Distance travelled: 
		distance = (velocity_actual[line]*time_step) + (0.5 * acceleration_actual[line]*time_step*time_step); 

		//fuel consumption: 
		if(Demand_torque[line]>0)
		{

			Power_demand =Instantaneous_power[line]*time_step; //W to J

			bsf = BrakeSpecificFuelConsumption[torque_counter][speed_counter]/(3.6 * 1000000); // (g/kWh) to (g/J)

			Instantaneous_FuelConsumption[line]= (Power_demand * bsf / 832);
				
			Total_FuelConsumption += Instantaneous_FuelConsumption[line];   // 832 g/L density of fuel
				
		}
		else
		{
			Instantaneous_FuelConsumption[line]=0.01436;    //calculated indling consumption 0.01436
			Total_FuelConsumption+=Instantaneous_FuelConsumption[line];
				
		}
		
		//cout<<"\n line finished: "<<line+1<<"distance: "<<total_distance<<"m velocity: "<<velocity_actual[line];
		Total_time += time_step;
		cycle_time[line]=Total_time;
		velocity_actual[line+1] =velocity_actual[line]+ acceleration_actual[line];
		total_distance += distance;
		line++;
		
		//cout<<"\n line : "<<(line+1)<<" distance covered: "<<total_distance;
		/*
		if(distance>5000 && flip_flag == 0)
		{
			cout<<"\ncrossed 5km, distance = "<<distance<<" at line: "<<line;
			flip_flag=1;
		}
		if(distance>10000 && flip_flag == 1)
		{
			cout<<"\ncrossed 10km, distance = "<<distance<<"m at line: "<<line;
			flip_flag=0;
		}
		if(distance>150000 && flip_flag == 0)
		{
			cout<<"\ncrossed 150km, distance = "<<distance<<" at line: "<<line;
			flip_flag=1;
		}
		if(distance>200000 && flip_flag == 1)
		{
			cout<<"\ncrossed 200km, distance = "<<distance<<" at line: "<<line;
			flip_flag=0;
		}
		if(distance>250000 && flip_flag == 0)
		{
			cout<<"\ncrossed 250km, distance = "<<distance<<" at line: "<<line;
			flip_flag=1;
		}
		*/
	}	//end while

	cout<<"\n The total fuel consumption= "<<Total_FuelConsumption<<" for the distance: "<<total_distance<<" which took: "<<Total_time<<" s";
	cout<<"\n deceleration exceeded -1 m/s2: "<<deceleration_flag;
	
	// Write output log to file:
	write_file(Instantaneous_power,velocity_actual,  acceleration_actual,  Selected_gear, Instantaneous_FuelConsumption,  cycle_time, Maximum_ResistiveForce,  Selected_engine_speed,  Demand_torque,  Road_ResistiveForce,distance_log,slope_log);
	_getch();

} // end main

