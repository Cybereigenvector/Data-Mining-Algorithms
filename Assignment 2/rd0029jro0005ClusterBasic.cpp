/////////////////////////////////////////////////////////////////////////////////
//This program parses the arff file and creates the data and attribute matrices//
//Then the data is normalized as per requirement							   //
//@Rishabh Das (rd0029)														   //
/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//The data_matrix and its dimensions are declared globally. 				  //
////////////////////////////////////////////////////////////////////////////////
vector< vector<float> > data_matrix;
vector<string> attribute;
vector< vector<float> > distance_matrix;
vector< vector<float> > cluster_centers;
vector < vector <float> > parameter_vector;


int column,row,num_attribute,index;
string last_attribute;

////////////////////////////////////////////////////////////////////////////////
//This function creates the data matrix and the attribute matrix 			  //	
//The matrix is implemented as a C++ vector. After the function is 			  //	
//invoked this function parses the .arff file and stroes the data 			  //	
//to the data_matrix the name of the attributes and the class is      		  //	
//stored in the attribute vector. when run=0 the function stores the 		  //
//dimension of the data matrix. When run=1 the values are stored in the 	  //	
//created vector.											  				  //
//////////////////////////////////////////////////////////////////////////////// 		
void create_data_matrix(int run,string filename)
{	
	//Each line read from the document is stored in the string 'line'
	string line;
	ifstream myfile (filename.c_str());

	column=0;
	row=0;
	num_attribute=0;

	last_attribute="";

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
////////////////////////////////////////////////////////////////////////////////
//This Function uses the data vector created  by the previous function 		  //
//to find the required parameters. The Mean and standard deviation is  		  //	    
//calculated. 													   			  //
////////////////////////////////////////////////////////////////////////////////
void Normalize(string filename)
{
	string line;
	bool data=true;

	vector <float> temp_vector;
	
	temp_vector.resize(column);

	parameter_vector.push_back(temp_vector);
	parameter_vector.push_back(temp_vector);

	ifstream readfile (filename.c_str());
	
	if (readfile.is_open())
	{
		while (getline(readfile,line))
		{
			int found_data=line.find("data");

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
////////////////////////////////////////////////////////////////////////////////
//This function creates the necessary output .arff files 					  //
////////////////////////////////////////////////////////////////////////////////
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
		output1 = output1 + loginname + "ClusterCenterNormalizedBasic" + filename.c_str();
		output2 = output2 + loginname + "ClusteringNormalizedBasic" + filename.c_str();
		output3 = output3 + loginname + "ClusterCenterUnnormalizedBasic" + filename.c_str();
		output4 = output4 + loginname + "ClusteringUnnormalizedBasic" + filename.c_str();
	}
	else
	{
		output1 = output1 + loginname + "ClusterCenterBasic" + filename.c_str();
		output2 = output2 + loginname + "ClusteringBasic" + filename.c_str();
	}
	
	ofstream outfile_Cluster_Center (output1.c_str());
	ofstream outfile_ClusteringBasic (output2.c_str());
	ofstream outfile_ClusterCenterUnnormalized(output3.c_str());
	ofstream outfile_ClusteringBasicUnnormalized(output4.c_str());
	if(norm)
	{
		
		outfile_ClusterCenterUnnormalized.is_open();
		outfile_ClusteringBasicUnnormalized.is_open();
	}

	outfile_Cluster_Center.is_open();
	outfile_ClusteringBasic.is_open();
		

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
				outfile_ClusteringBasic<<line;
				if(norm)
				{
					outfile_ClusterCenterUnnormalized<<line;
					outfile_ClusteringBasicUnnormalized<<line;
				}
			}	
			if(found_data !=std::string::npos)
			{
				data=false;
				outfile_ClusteringBasic<<"@attribute cluster real";
				outfile_ClusteringBasic<<endl;
				outfile_ClusteringBasic<<endl;
				outfile_ClusteringBasic<<"@data";
				outfile_ClusteringBasic<<endl;

				outfile_Cluster_Center<<"@attribute cluster real";
				outfile_Cluster_Center<<endl;
				outfile_Cluster_Center<<endl;
				outfile_Cluster_Center<<"@data";
				outfile_Cluster_Center<<endl;

				if(norm)
				{
					outfile_ClusteringBasicUnnormalized<<"@attribute cluster real";
					outfile_ClusteringBasicUnnormalized<<endl;
					outfile_ClusteringBasicUnnormalized<<endl;
					outfile_ClusteringBasicUnnormalized<<"@data";
					outfile_ClusteringBasicUnnormalized<<endl;

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
			outfile_Cluster_Center<<setw(13)<<cluster_centers[i][j];
		}
		outfile_Cluster_Center<<setw(13)<<i+1;
		outfile_Cluster_Center<<endl;
					
	} 
	//For Clustered Data
	for(int i=0;i<data_matrix.size();i++)
	{
		for(int j=0;j<data_matrix[i].size();j++)
		{
			outfile_ClusteringBasic<<setw(13)<<data_matrix[i][j];
		}
		outfile_ClusteringBasic<<setw(13)<<distance_matrix[i][val_k]+1;
		outfile_ClusteringBasic<<endl;
	}
		outfile_ClusteringBasic.close();
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
					outfile_ClusterCenterUnnormalized<<setw(13)<<(cluster_centers[i][j]*parameter_vector[1][j])+parameter_vector[0][j];
				}
				else
				{
					outfile_ClusterCenterUnnormalized<<setw(13)<<cluster_centers[i][j];
				}
			}
			outfile_ClusterCenterUnnormalized<<setw(13)<<i+1;
			outfile_ClusterCenterUnnormalized<<endl;
					
		} 
		//For Clustered Data
		for(int i=0;i<data_matrix.size();i++)
		{
			for(int j=0;j<data_matrix[i].size();j++)
			{
				if(j==index)
				{
					outfile_ClusteringBasicUnnormalized<<setw(13)<<data_matrix[i][j];
				}
				else
				{
					outfile_ClusteringBasicUnnormalized<<setw(13)<<(data_matrix[i][j]*parameter_vector[1][j])+parameter_vector[0][j];
				}
				
			}
			outfile_ClusteringBasicUnnormalized<<setw(13)<<distance_matrix[i][val_k]+1;
			outfile_ClusteringBasicUnnormalized<<endl;
		}
   		outfile_ClusteringBasicUnnormalized.close();
   		outfile_ClusterCenterUnnormalized.close();
   	}
}
////////////////////////////////////////////////////////////////////////////////
//This funtion prints the clustered data 								      //
////////////////////////////////////////////////////////////////////////////////
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
				cout<<setw(10)<<distance_matrix[i][val_k]+1<<endl;
			}
		}
	}

}
////////////////////////////////////////////////////////////////////////////////
//This function of this function is to print the cluster centers 			  //
////////////////////////////////////////////////////////////////////////////////
void print_cluster_center()
{
	cout<<"\nThe Cluster centers are "<<endl;
	for(int i=0;i<cluster_centers.size();i++)
  	{
  		for(int j=0;j<cluster_centers[i].size();j++)
  		{
  			if(j!=index)
  			{
  				cout<<setw(10)<<cluster_centers[i][j]<<"  ";
  			}
  		}	
  		cout<<endl;
  	}
}
////////////////////////////////////////////////////////////////////////////////
//This function is finding the class index,i.e the part that is skipped during//
//normalization.If all value are to be normalized the index is set out of     // 
//range of the attribute Number 											  //
//////////////////////////////////////////////////////////////////////////////// 
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

////////////////////////////////////////////////////////////////////////////////
//The Function is used to initialize the cluster centers when the K-Menas is  //
//running for the first time                 								  //
////////////////////////////////////////////////////////////////////////////////
void initialize_cluster_center(int val_k)
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
				if(temp_vector[i]==random)
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
////////////////////////////////////////////////////////////////////////////////
//This function makes the empty distance matrix as per the required dimensions//       
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
//This function makes the empty cluster center matrix as per the required     //
//dimensions 																  // 
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
//This function calculates the Euclidean distance as per the given cluster    // 
//center and the row number of the data matrix. The float value is returned   //
//to the calling function.	         										  //
////////////////////////////////////////////////////////////////////////////////
void calculate_distance_euclidean(int val_k)
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
////////////////////////////////////////////////////////////////////////////////
//This function calls the distance function and finds the lowest distance     //
//from the cluster center to the point                                        //
////////////////////////////////////////////////////////////////////////////////
bool find_cluster_center(int val_k)
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
			if(distance_matrix[j][val_k]==i)
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
////////////////////////////////////////////////////////////////////////////////
//This Function finds the lowest distance and designates the clsuter of the   //
//point                                                                       //
////////////////////////////////////////////////////////////////////////////////
void find_lowest_distance(int val_k)
{
	float lowest,low_index;

	for(int i=0;i<row;i++)
	{	
		low_index=0;
		for(int j=0;j< val_k+1;j++)
		{
			if(j==0)
			{
				lowest=distance_matrix[i][j];
			}
			else if(j==val_k)
			{
				distance_matrix[i][j]=low_index;
			}
			else if (distance_matrix[i][j]<=lowest)
			{
				lowest=distance_matrix[i][j];
				low_index=j;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//This algorithm puts together all the small function to make the K-Means     //
//algorithm work   															  //
////////////////////////////////////////////////////////////////////////////////
void K_means_basic(int No_of_cluster)
{
  bool change;

  cout<<"Starting K_means algorithm!!"<<endl;
  
  make_distance_matrix(No_of_cluster);
  make_cluster_center_matrix(No_of_cluster);
  initialize_cluster_center(No_of_cluster);
  calculate_distance_euclidean(No_of_cluster);
  find_lowest_distance(No_of_cluster);
  
  change=true;

  while(change)
  {
  	change=find_cluster_center(No_of_cluster);
 	calculate_distance_euclidean(No_of_cluster);
 	find_lowest_distance(No_of_cluster);
  }
  print_clustered_data(No_of_cluster);
  print_cluster_center();

}
////////////////////////////////////////////////////////////////////////////////
//This is the main function. The commandline arguments are parsed and  handled//
//in this function. After the required arguments are provided by the user this//
//function calls the required function to perform the required operation  	  //
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	string file,class_name,K;
	bool file_found,class_found,k_found,normalize;
	
	vector <string> command_line;

	file_found=false;
	class_found=false;
	k_found=false;
	normalize=false;

////////////////////////////////////////////////////////////////////////////////
//The section of the main fucntion parses the commandline input and assigns   //
//the values to appropriate variable										  //
////////////////////////////////////////////////////////////////////////////////

	class_name="N/A";
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


////////////////////////////////////////////////////////////////////////////////
//This segment of the main function is used to call all the required functions//
//to perform the required functionality                                       //
////////////////////////////////////////////////////////////////////////////////

	//Create_data_matrix(first run gets the dimension of the data while the second run parses the data and creates the data matrix,File name to be read)
	create_data_matrix(0,file.c_str());
	find_class_index(class_found,class_name);

	if(normalize)
	{
		//create_outputfile(filename that is to be read,boolean-tells if the class attribute was provided or not,if yes the provided class name)
		Normalize(file.c_str());
	}
	K_means_basic(atoi(K.c_str()));
	create_outputfile(file.c_str(),atoi(K.c_str()),normalize);

	return 0; 
}