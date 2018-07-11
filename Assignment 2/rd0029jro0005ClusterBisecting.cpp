///////////////////////////////////////////////////////////////////////////////////////
//This program parses the arff file and implements the bisecting K-Means algorithm.  //
//This program does not use any arff library and has the capability to parse arff    //
//data independently. As per requirement the identification of class attribute and   //
//the normalize functionality is also incorporated in the program.   				 //
//@Rishabh Das (rd0029)														         //
///////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////
//The data_matrix and its dimensions are declared globally. 			             //
///////////////////////////////////////////////////////////////////////////////////////
vector< vector<float> > data_matrix;
vector<string> attribute;
vector< vector<float> > distance_matrix;
vector< vector<float> > cluster_centers;
vector < vector <float> > parameter_vector;

int column,row,num_attribute,index,current_cluster_size;

///////////////////////////////////////////////////////////////////////////////////////
//This function creates the data matrix and the attribute matrix 			         //	
//The matrix is implemented as a C++ vector. After the function is 			         //	
//invoked this function parses the .arff file and stores the data 			         //	
//to the data_matrix the name of the attributes and the class is      		         //	
//stored in the attribute vector. when run=0 the function stores the 		         //
//dimension of the data matrix. When run=1 the values are stored in the 	         //	
//created vector.											  				  		 //
/////////////////////////////////////////////////////////////////////////////////////// 		
void create_data_matrix(int run,string filename)
{	
	//Each line read from the document is stored in the string 'line'
	string line;
	ifstream myfile (filename.c_str());

	column=0;
	row=0;
	num_attribute=0;

	bool startparser= false;

	//Checking for file openning error
	if (myfile.is_open())
	{
		 //getline is used to read and store one entire line from the document to the string variable 'line' 
		 while (getline(myfile,line))
		 {
		 	int length=0;
		 	int position=0;
		 	int consecutive_space=0;

		 	//find function is used to detect the word "data" and "attributes"
		 	int found_data=line.find("data");
		 	int found_attribute=line.find("attribute");

		 	bool start_count=false;

		 	float number_final=0;
   			 	   			 	   			 	
		 	string number="";
		 	string att="";
		 	
		 	column=0;
		 	length=line.length();

		 	//If the word "attribute" is found in the line
		 	//this section parses the the line to find the  
		 	//name of the attribute
		 	if(found_attribute !=std::string::npos)
   		 	{
   		 		num_attribute++;

   		 		if(run==1)
   		 		{
   		 			position=line.rfind("attribute")+9;
   		 			while(line.at(position)==' ')
		 			{
		 				position++;
		 			}
		 			while(line.at(position)!=' ')
		 			{
		 				att+=line.at(position);
		 				position++;
		 			}
		 			attribute[num_attribute-1]=att;	
		 			att=""; 
		 		
   		 		}
   		 	}
   			// This section parses the data and creates the 2D vector 	  			 	
		 	while(position<length && startparser==true)
		 	{
		 		vector<float> row_matrix;
		 		while(line.at(position)==' ')
		 		{
		 			if(consecutive_space==0 && start_count==true)
		 			{
		 				column++;
		 			}
		 			position++;
		 			consecutive_space++;
		 		}
		 		consecutive_space=0;
		 		if(line.at(position)!=' ')
		 		{
		 			start_count=true;
   		 			number+=line.at(position);
   		 			if(position!=(length-1) && line.at(position+1)==' ' && run==1)
   		 			{
   		 				number_final=atof(number.c_str());
   		 				number="";
   		 				data_matrix[row][column]=number_final;
   		 		    }
   		 		}
   		 		if(position==(length-1))
   		 		{
   		 			column++;	
   		 			row++;
   		 			number_final=atof(number.c_str());
   		 			number="";
   		 			if(run==0)
   		 			{
   		 				row_matrix.resize(column);
   		 				data_matrix.push_back(row_matrix);
   		 			}
   		 			else if(run==1)
   		 			{
   		 				data_matrix[row-1][column-1]=number_final;
   		 			}
   		 		}
   		 		
   		 		position++;
   		 	}
   		 	//Enables the data parser if the word "data" is found in the line
   		 	if(found_data !=std::string::npos)
   		 	{
   		 		startparser=true;
   		 	}
   		 }
    }
    else
    {
    	cout<<"Error openning File"<<endl;
    }
    myfile.close();
    if(run==0)
   	{
   		attribute.resize(num_attribute);
    	create_data_matrix(1,filename.c_str());	
    }
    else if(run==1)
    {
    	cout<<endl<<"The data read from file is as follows:-"<<endl;
    	for(int i=0;i<data_matrix.size();i++)
    	{
    		for(int j=0;j<data_matrix[i].size();j++)
    		{
    			cout <<data_matrix[i][j]<<"  ";
    		}
    		cout<<endl;
    	}
    	cout<<endl<<"The data matrix was generated!!"<<endl;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
//This Function uses the data vector created  by the previous function 		         //
//to find the required parameters. The Mean and standard deviation is  		         //	    
//calculated and the normalization is done by this function if needed  	             //
///////////////////////////////////////////////////////////////////////////////////////
void Normalize(string filename)
{
	string line;
	bool data=true;

	vector <float> temp_vector;

	temp_vector.resize(column);

	parameter_vector.push_back(temp_vector);
	parameter_vector.push_back(temp_vector);

	ifstream readfile (filename.c_str());

	//The name sof the outputfile
	//output1->Output file with the mean and Standard deviation
	//output2->Output file with the normalized values 

	if (readfile.is_open())
	{
		while (getline(readfile,line))
		{
			int found_data=line.find("data");
			
			//keeps copying the content of the input file until the 
			//word data is found
			if(found_data !=std::string::npos)
   		 	{
   		 		data=false;
   		 		//Finding the mean of all the attributes
   		 		//parsed in the data file
   		 		for(int i=0;i<2;i++)
				{
					for(int j=0;j<column;j++)
					{
						parameter_vector[i][j] =0;
					}
					
				}
   		 		for(int i=0;i<data_matrix.size();i++)
				{
					for(int j=0;j<column;j++)
					{
						parameter_vector[0][j] += data_matrix[i][j];
					}
				}
				for(int j=0;j<column;j++)
				{
					parameter_vector[0][j] = parameter_vector[0][j] / row; 
   		 		}
   		 		//Finding the standard deviation of all the attributes
   		 		//in the parsed data
   		 		for(int i=0;i<data_matrix.size();i++)
				{
					for(int j=0;j<column;j++)
					{
						parameter_vector[1][j] +=pow(data_matrix[i][j] -parameter_vector[0][j],2);
					}
					
				}
				for(int j=0;j<column;j++)
				{
					parameter_vector[1][j] = parameter_vector[1][j] / row; 
					parameter_vector[1][j] = sqrt(parameter_vector[1][j]);
   		 		}

				//Normalizing the vectors
				for(int i=0;i<data_matrix.size();i++)
				{
					for(int j=0;j<data_matrix[i].size();j++)
					{
						if(j!=index)
						{
							data_matrix[i][j]= ((data_matrix[i][j]-parameter_vector[0][j])/ parameter_vector[1][j]);
						}
					}
					
				}
   		 	}


		}
	}
	else
	{
		cout<<"Error openning file!!"<<endl;
	}
	readfile.close();
}
///////////////////////////////////////////////////////////////////////////////////////
//This function creates the necessary output .arff files 							 //
///////////////////////////////////////////////////////////////////////////////////////
void create_outputfile(string filename,int val_k,bool norm)
{
	bool data;
	string output1,output2,output3,output4,loginname,line;

	data=true;

	//The name sof the outputfile
	//output1->Output file with the Cluster centers
	//output2->Output file with the cluster results
	//output3->Output file with the inversed normalized cluster center
	//output4->Output file with re-mapped data and cluster points
	loginname="rd0029jro0005";
	output1="";
	output2="";

	if(norm)
	{
		output1 = output1 + loginname + "ClusterCenterNormalizedBisecting" + filename.c_str();
		output2 = output2 + loginname + "ClusteringNormalizedBisecting" + filename.c_str();
		output3 = output3 + loginname + "ClusterCenterUnnormalizedBisecting" + filename.c_str();
		output4 = output4 + loginname + "ClusteringUnnormalizedBisecting" + filename.c_str();
	}
	else
	{
		output1 = output1 + loginname + "ClusterCenterBisecting" + filename.c_str();
		output2 = output2 + loginname + "ClusteringBisecting" + filename.c_str();
	}
	
	ofstream outfile_Cluster_Center (output1.c_str());
	ofstream outfile_ClusteringBisecting (output2.c_str());
	ofstream outfile_ClusterCenterUnnormalized(output3.c_str());
	ofstream outfile_ClusteringBisectingUnnormalized(output4.c_str());
	if(norm)
	{
		outfile_ClusterCenterUnnormalized.is_open();
		outfile_ClusteringBisectingUnnormalized.is_open();
	}

	outfile_Cluster_Center.is_open();
	outfile_ClusteringBisecting.is_open();
		

	ifstream readfile (filename.c_str());
	
	if (readfile.is_open())
	{
		while (getline(readfile,line))
		{
			int found_data=line.find(attribute[num_attribute-1]);
			//keeps copying the content of the input file until the 
			//last attribute is found
			if(data)
			{
				outfile_Cluster_Center<<line;
				outfile_ClusteringBisecting<<line;
				if(norm)
				{
					outfile_ClusterCenterUnnormalized<<line;
					outfile_ClusteringBisectingUnnormalized<<line;
				}
			}	
			if(found_data !=std::string::npos)
			{
				data=false;
				outfile_ClusteringBisecting<<"@attribute cluster real";
				outfile_ClusteringBisecting<<endl;
				outfile_ClusteringBisecting<<endl;
				outfile_ClusteringBisecting<<"@data";
				outfile_ClusteringBisecting<<endl;

				outfile_Cluster_Center<<"@attribute cluster real";
				outfile_Cluster_Center<<endl;
				outfile_Cluster_Center<<endl;
				outfile_Cluster_Center<<"@data";
				outfile_Cluster_Center<<endl;

				if(norm)
				{
					outfile_ClusteringBisectingUnnormalized<<"@attribute cluster real";
					outfile_ClusteringBisectingUnnormalized<<endl;
					outfile_ClusteringBisectingUnnormalized<<endl;
					outfile_ClusteringBisectingUnnormalized<<"@data";
					outfile_ClusteringBisectingUnnormalized<<endl;

					outfile_ClusterCenterUnnormalized<<"@attribute cluster real";
					outfile_ClusterCenterUnnormalized<<endl;
					outfile_ClusterCenterUnnormalized<<endl;
					outfile_ClusterCenterUnnormalized<<"@data";
					outfile_ClusterCenterUnnormalized<<endl;

				}

			}
		}
	}
    //For the cluster centers
	for(int i=0;i<cluster_centers.size();i++)
	{
		for(int j=0;j<cluster_centers[i].size();j++)
		{
			outfile_Cluster_Center<<setw(10)<<cluster_centers[i][j];
		}
		outfile_Cluster_Center<<setw(10)<<i+1;
		outfile_Cluster_Center<<endl;
					
	} 
	//For Clustered Data
	for(int i=0;i<data_matrix.size();i++)
	{
		for(int j=0;j<data_matrix[i].size();j++)
		{
			outfile_ClusteringBisecting<<setw(10)<<data_matrix[i][j];
		}
		outfile_ClusteringBisecting<<setw(10)<<distance_matrix[i][2]+1;
		outfile_ClusteringBisecting<<endl;
	}
		outfile_ClusteringBisecting.close();
   		outfile_Cluster_Center.close();
   	if(norm)
   	{
   		//For the cluster centers
		for(int i=0;i<cluster_centers.size();i++)
		{
			for(int j=0;j<cluster_centers[i].size();j++)
			{
				if(j!=index)
				{
					outfile_ClusterCenterUnnormalized<<setw(10)<<(cluster_centers[i][j]*parameter_vector[1][j])+parameter_vector[0][j];
				}
				else
				{
					outfile_ClusterCenterUnnormalized<<setw(10)<<cluster_centers[i][j];
				}
			}
			outfile_ClusterCenterUnnormalized<<setw(10)<<i+1;
			outfile_ClusterCenterUnnormalized<<endl;
					
		} 
		//For Clustered Data
		for(int i=0;i<data_matrix.size();i++)
		{
			for(int j=0;j<data_matrix[i].size();j++)
			{
				if(j!=index)
				{
					outfile_ClusteringBisectingUnnormalized<<setw(10)<<(data_matrix[i][j]*parameter_vector[1][j])+parameter_vector[0][j];
				}
				else
				{
					outfile_ClusteringBisectingUnnormalized<<setw(10)<<data_matrix[i][j];
				}
			}
			outfile_ClusteringBisectingUnnormalized<<setw(10)<<distance_matrix[i][2]+1;
			outfile_ClusteringBisectingUnnormalized<<endl;
		}
   		outfile_ClusteringBisectingUnnormalized.close();
   		outfile_ClusterCenterUnnormalized.close();
   	}
}
///////////////////////////////////////////////////////////////////////////////////////
//This funtion prints the clustered data. The data matrix along with the cluster 	 //
//number is showed by this function 												 //
///////////////////////////////////////////////////////////////////////////////////////
void print_clustered_data(int val_k)
{
	for(int i=0;i<data_matrix.size();i++)
	{
		for(int j=0;j<data_matrix[i].size();j++)
		{
			if (j+1 !=data_matrix[i].size())
			{
				cout<<setw(10)<<data_matrix[i][j];
			}
			else
			{
				cout<<setw(10)<<data_matrix[i][j];
				cout<<setw(10)<<distance_matrix[i][val_k]<<endl;
			}
		}
	}

}
///////////////////////////////////////////////////////////////////////////////////////
//This function prints the cluster centers 											 //
///////////////////////////////////////////////////////////////////////////////////////
void print_cluster_center()
{
	cout<<"\nThe Cluster centers are "<<endl;
	for(int i=0;i<cluster_centers.size();i++)
  	{
  		for(int j=0;j<cluster_centers[i].size();j++)
  		{
  			cout<<cluster_centers[i][j]<<"  ";
  		}	
  		cout<<endl;
  	}
}
///////////////////////////////////////////////////////////////////////////////////////
//This function is finding the class index,i.e the part that is skipped during 	     //
//normalization.If all value are to be normalized the index is set out of range of   //	 
//the attribute number															     //
/////////////////////////////////////////////////////////////////////////////////////// 
void find_class_index(bool normalize_class, string clss_name)
{
	if(normalize_class)
	{
		for(index=0;index<attribute.size();index++)
		{
			if(clss_name.compare(attribute[index])==0)
			{
				break;
			}
		}
		cout<<"The index of the class is "<<index<<endl;
	}
	else
	{
		index=attribute.size()+10;
		cout<<"The index of the class is set out of bound "<<index<<endl;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// This function randomly initializes the cluster centers in a given cluster. During //
//the first run the cluster number is set to -1. This signnifies that the 			 //
//initialization is to done from the entire data set rather than from a specified 	 //
//cluster																			 //
///////////////////////////////////////////////////////////////////////////////////////
void initialize_cluster_center(int val_k,int cluster_number)
{
	int random;
	float ini;
	bool present;
	vector<int> temp_vector;

	temp_vector.resize(val_k);
	

	cout<<"The number of samples are "<<row;

	if(val_k>row)
	{
		cout<<"The amount of data is less\nPlease enter more data!!"<<endl;
		exit(1);
	}
	else
	{
		ini=0;

		cout<<endl<<"The initial cluster centers are:-"<<endl;
		
		while(ini!=val_k)
		{
			present=false;
			srand(time(NULL));
			random=rand();
			random= random%row;

			for(int i=0;i<temp_vector.size();i++)
			{
				if(temp_vector[i]==random )
				{
					present=true;
				}
				if(distance_matrix[random][val_k]!=cluster_number && current_cluster_size!=1)
				{
					present=true;
				}
			}
			if(!present)
			{
				for(int i=0;i<column;i++)
				{
					cluster_centers[ini][i]=data_matrix[random][i];
					temp_vector[ini]=random;
				}
				ini++;
			}


		}
		print_cluster_center();
	}
}
///////////////////////////////////////////////////////////////////////////////////////
//This function makes the empty distance matrix as per the required dimensions. In   //
//bisecting K-Means the distance matrix is made only once and the distances are		 //
//recalculated and replaced whenever any other run is made  					     // 
///////////////////////////////////////////////////////////////////////////////////////
void make_distance_matrix(int value_of_k)
{
	vector <float> temp_vector;
	temp_vector.resize(value_of_k+1);
	for(int i=0;i<row;i++)
	{
		distance_matrix.push_back(temp_vector);
	}
	cout<<"Distance matrix created with the dimensions "<<distance_matrix.size()<<","<<distance_matrix[0].size()<<endl;
}
///////////////////////////////////////////////////////////////////////////////////////
//This function makes the empty cluster center matrix as per the required dimensions.// 
//In the bisecting K-Means algorithm the cluster matrix is created only once and the //
//same cluster matrix is reused whenever the cluster center is  modified 			 // 
///////////////////////////////////////////////////////////////////////////////////////
void make_cluster_center_matrix(int value_of_k)
{
	vector <float> temp_vector;
	temp_vector.resize(column);
	for(int i=0;i<value_of_k;i++)
	{
		cluster_centers.push_back(temp_vector);
	}
	cout<<"Cluster center matrix created with the dimensions "<<cluster_centers.size()<<","<<cluster_centers[0].size()<<endl;
}
///////////////////////////////////////////////////////////////////////////////////////
//This function calculates the Euclidean distance as per the given cluster center    //
//and the row number of the data matrix. This distance is stored in a separate 		 //
//distance matrix 																     //
///////////////////////////////////////////////////////////////////////////////////////
void calculate_distance_euclidean(int val_k, int cluster_number)
{
	float distance=0;

	for(int i=0;i<distance_matrix.size();i++)
	{	
		for(int k=0;k<val_k;k++)
		{
			for (int j=0;j<data_matrix[i].size();j++)
			{
				if(j!=index)
				{
					distance+= pow((cluster_centers[k][j]-data_matrix[i][j]),2);
				}
			}
			distance_matrix[i][k]=sqrt(distance);
			distance=0;
		}		
	} 
}
///////////////////////////////////////////////////////////////////////////////////////
//This function calls the distance function and finds the lowest distance from the 	 //
//cluster center to the point 														 //
///////////////////////////////////////////////////////////////////////////////////////
bool find_cluster_center(int val_k,int cluster_number)
{
	vector <float> temp_vector;
	bool cluster_change;
	float count;

	cluster_change=false;
	temp_vector.resize(column);

	for(int i=0;i<val_k;i++)
	{
		count=0;
		for(int j=0;j<row;j++)
		{
			if(cluster_number==-1)
			{
				if(distance_matrix[j][2]==i)
				{
					count++;
					for(int k=0;k<column;k++)
					{
						temp_vector[k]+=data_matrix[j][k];
					}
				}
			}
			else if(distance_matrix[j][val_k]==cluster_number && i==0)
			{
				count++;
				for(int k=0;k<column;k++)
				{
					temp_vector[k]+=data_matrix[j][k];
				}
			}
			else if(distance_matrix[j][val_k]==current_cluster_size && i==1)
			{
				count++;
				for(int k=0;k<column;k++)
				{
					temp_vector[k]+=data_matrix[j][k];
				}
			}
		}
		for(int j=0;j<column;j++)
		{
			if(count==0)
			{
				count=1;
			}
			temp_vector[j]=temp_vector[j]/count;
			if(cluster_centers[i][j]!=temp_vector[j])
			{
				cluster_change=true;
			}
			cluster_centers[i][j]=temp_vector[j];
			temp_vector[j]=0;
		}
	}
	return cluster_change;
}
///////////////////////////////////////////////////////////////////////////////////////
//This Function finds the lowest distance and designates the cluster of the point    //
///////////////////////////////////////////////////////////////////////////////////////
void find_lowest_distance(int val_k,int cluster_number)
{
	float lowest,low_index;
	
	for(int i=0;i<row;i++)
	{	
		low_index=0;
		if(distance_matrix[i][val_k]==cluster_number || current_cluster_size==1)
		{
			for(int j=0;j< val_k+1;j++)
			{
				if(j==0)
				{
					lowest=distance_matrix[i][j];
				}
				else if(j==val_k)
				{
					if(low_index==0)
					{
						if(cluster_number==-1)
						{
							distance_matrix[i][j]=0;	
						}
						else
						{
							distance_matrix[i][j]=cluster_number;
						}
					}
					else if(low_index==1)
					{
						distance_matrix[i][j]=current_cluster_size;
					}
				}
				else if (distance_matrix[i][j]<=lowest)
				{
					lowest=distance_matrix[i][j];
					low_index=j;
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////
//This function returns the SSE value for the mentioned cluster.					 //	
///////////////////////////////////////////////////////////////////////////////////////
float SSE(int cluster_number)
{
	vector <float> mean;
	float count,SSE_value;

	mean.resize(column);
	count=0;
	SSE_value=0;

	for(int i=0;i<data_matrix.size();i++)
	{
		for(int j=0;j<column;j++)
		{
			if (cluster_number== distance_matrix[i][2])
			{
				mean[j]=mean[j]+ data_matrix[i][j];
				count++;
			} 
		}
		
	}
	count=count/column;
	//Finding the mean
	for(int i=0;i<mean.size();i++)
	{
		mean[i]=mean[i]/count;
	}
	//Finding the SSE
	for(int i=0;i<data_matrix.size();i++)
	{
		for(int j=0;j<column;j++)
		{
			if (cluster_number== distance_matrix[i][2])
			{
				SSE_value=SSE_value+ pow(data_matrix[i][j]-mean[j],2); 
			} 
		}
		
	}
	return SSE_value;

}
///////////////////////////////////////////////////////////////////////////////////////
//This function finds the highest SSE and decides which cluster is to divided further//
///////////////////////////////////////////////////////////////////////////////////////
float find_highest_SSE()
{	
	float highest,high_index;

	highest=SSE(0);
	high_index=0;

	for(int i=0;i<current_cluster_size;i++)
	{
		if(highest<SSE(i))
		{
			highest=SSE(i);
			high_index=i;	
		}
	}
	cout<<"The cluster ->"<<high_index<< " has to be divided!"<<endl;
	return high_index;
}
///////////////////////////////////////////////////////////////////////////////////////
//This algorithm puts together all the small function to make the K-Means algorithm  //
//work,If the cluster to bisect is -1 then the algorithm will run the K-Mean on the  //
//entire data set, on the contrary if the cluster number to bisect is specified then // 
//the algorithm will confine the bisection within the clsuter itself				 //
///////////////////////////////////////////////////////////////////////////////////////
void K_means(int No_of_cluster,int cluster_to_bisect)
{
  bool change;

  cout<<"Starting Bisecting K_means algorithm!!"<<"The cluster to bisect is ->> "<<cluster_to_bisect<<endl;
  
  if(current_cluster_size==1)
  {
  	make_distance_matrix(No_of_cluster);
  	make_cluster_center_matrix(No_of_cluster);
  }
  initialize_cluster_center(No_of_cluster,cluster_to_bisect);
  calculate_distance_euclidean(No_of_cluster,cluster_to_bisect);
  find_lowest_distance(No_of_cluster,cluster_to_bisect);

  change=true;

  while(change)
  {
  	change=find_cluster_center(No_of_cluster,cluster_to_bisect);
 	calculate_distance_euclidean(No_of_cluster,cluster_to_bisect);
 	find_lowest_distance(No_of_cluster,cluster_to_bisect);
  }
  current_cluster_size++;
  print_clustered_data(No_of_cluster);  

}
///////////////////////////////////////////////////////////////////////////////////////
//This functions calls the required functions to cluster the data set using 	     //
//bisecting k means algorithm													     //
///////////////////////////////////////////////////////////////////////////////////////
void K_means_bisecting(int No_of_cluster)
{
	K_means(2,-1);
	while (current_cluster_size< No_of_cluster)
	{
		K_means(2,find_highest_SSE());
	}
	cluster_centers.clear();
	make_cluster_center_matrix(No_of_cluster);
	find_cluster_center(No_of_cluster,-1);	
}
///////////////////////////////////////////////////////////////////////////////////////
//This is the main function. The commandline arguments are parsed and  handled in    //
//this function. After the required arguments are provided by the user this 	     //
//function calls the required function to perform the required operation  		   	 //
///////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	string file,class_name,K;
	bool file_found,class_found,k_found,normalize;
	
	vector <string> command_line;

	file_found=false;
	class_found=false;
	k_found=false;
	normalize=false;
	current_cluster_size=1;

///////////////////////////////////////////////////////////////////////////////////////
//The section of the main fucntion parses the commandline input and assigns the      //
//values to appropriate variable												     //
///////////////////////////////////////////////////////////////////////////////////////

	class_name="";
	K="";

	if (argc > 8 || argc < 5)
	{
		cout <<"./rd0029Normalize -i <Inputfile> -c <classattribute> -K N -normalize"<<endl;
		exit(1);
	}
	//storing the arv values to a string vector
	command_line.assign(argv+1,argv+argc);
	
	//parsing andchecking the command line arguments
	for(int i=0;i<command_line.size()-1;i++)
	{
		if(command_line[i].compare("-i")==0 && i+1 <command_line.size())
		{
			file=command_line[i+1];
			file_found=true;
		}
		else if (command_line[i].compare("-c")==0 && i+1 <command_line.size())
		{
			class_name=command_line[i+1];
			class_found=true;
		}
		else if (command_line[i].compare("-K")==0 && i+1 <command_line.size())
		{
			K=command_line[i+1];
			k_found=true;
		}	
		else if (command_line[i].compare("-normalize")==0 || i+1 <command_line.size())
		{
			if(command_line[i+1].compare("-normalize")==0)
			{
				normalize=true;
			}
			else if (command_line[0].compare("-normalize")==0)
			{
				normalize=true;
			}
		}
	}
	//Checking if the file name was found or not
	if(!file_found || !k_found )
	{
		cout<<"Plese check the inputs!!" <<endl<<"./rd0029Normalize -i <Inputfile> -c <classattribute> -K N -normalize"<<endl;
		exit(1);
	}
	//Checking if the class name was found or not

	cout<<"The file name is->"<<file<<endl<<"Number of clusters->"<<K<<endl<<"Class attribute->"<<class_name<<endl<<"Normalizing->"<<normalize<<endl;


///////////////////////////////////////////////////////////////////////////////////////
//This segment of the main function is used to call all the required functions to    //
//perform the required functionality                                                 //
///////////////////////////////////////////////////////////////////////////////////////

	//Create_data_matrix(first run gets the dimension of the data while the second run parses the data and creates the data matrix,File name to be read)
	create_data_matrix(0,file.c_str());
	//This function finds the index of the class attribute which has to Skipped during normalization
	find_class_index(class_found,class_name);

	if(normalize)
	{
		//Normalize(filename that is to be read)
		Normalize(file.c_str());
	}
	K_means_bisecting(atoi(K.c_str()));
	create_outputfile(file.c_str(),atoi(K.c_str()),normalize);

	return 0; 
}