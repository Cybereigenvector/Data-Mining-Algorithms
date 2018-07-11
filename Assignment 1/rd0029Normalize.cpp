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
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//The data_matrix and its dimensions are declared globally. 				  //
////////////////////////////////////////////////////////////////////////////////
vector< vector<float> > data_matrix;
vector<string> attribute;
int column,row,num_attribute;

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
    	cout<<"The files were generated!!"<<endl;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//This Function uses the data vector created  by the previous function 			//
//to find the required parameters. The Mean and standard deviation is  			//	    
//calculated and the two output files are created by this function.	   			//
//////////////////////////////////////////////////////////////////////////////////
void create_outputfile(string filename,bool normalize_class, string clss_name)
{
	string line,loginname,output1,output2;
	bool data=true;
	int index;

	vector <float> temp_vector;
	vector < vector <float> > parameter_vector;

	temp_vector.resize(column);

	parameter_vector.push_back(temp_vector);
	parameter_vector.push_back(temp_vector);

	ifstream readfile (filename.c_str());

	//The name sof the outputfile
	//output1->Output file with the mean and Standard deviation
	//output2->Output file with the normalized values 
	loginname="rd0029";
	output1="";
	output2="";

	output1 = output1 + loginname + "MeanStd" + filename.c_str();
	output2 = output2 + loginname + "Normalize" + filename.c_str();

	ofstream outfile_MeanStd (output1.c_str());
	ofstream outfile_Norm (output2.c_str());


	if (readfile.is_open())
	{
		while (getline(readfile,line))
		{
			int found_data=line.find("data");

			outfile_MeanStd.is_open();
			outfile_Norm.is_open();
			
			//keeps copying the content of the input file until the 
			//word data is found
			if(data)
			{
				outfile_Norm<<line;
				outfile_MeanStd<<line;
			}
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

   		 		for(int i=0;i<2;i++)
				{
					for(int j=0;j<data_matrix[i].size();j++)
					{
						outfile_MeanStd<<setw(11)<<parameter_vector[i][j];
					}
					outfile_MeanStd<<endl;
					
				} 
				//Finding the class index, This part finds which 
				//value to skip for the normalization. If all value 
				//are to be normalized the index is set out of 
				//range of the attribute number
				if(normalize_class)
				{
					for(index=0;index<attribute.size();index++)
					{
						if(clss_name.compare(attribute[index])==0)
						{
							break;
						}
					}
				}
				else
				{
					index=attribute.size()+1;
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
						outfile_Norm<<setw(11)<<data_matrix[i][j];
					}
					outfile_Norm<<endl;
					
				}
   		 		outfile_MeanStd.close();
   		 		outfile_Norm.close();
   		 	}


		}
	}
	else
	{
		cout<<"Error openning file!!"<<endl;
	}
	readfile.close();
}
/////////////////////////////////////////////////////////////////////////////////////
//This is the main function. The commandline arguments are parsed and  handled in  //
//this function. After the required arguments are provided by the user this 	   //
//function calls the required function to perform the required operation  		   //
/////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	string file,class_name;
	bool file_found,class_found;
	
	vector <string> command_line;

	file_found=false;
	class_found=false;

	class_name="";
	if (argc > 5 || argc < 3)
	{
		cout <<"./rd0029Normalize -i <Inputfile> -c <classattribute>"<<endl;
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
		else if (i+1 >command_line.size())
		{
			cout <<"./rd0029Normalize -i <Inputfile> -c <classattribute>"<<endl;
			exit(1);
		}
	}
	//Checking if the file name was found or not
	if(!file_found)
	{
		cout<<"Plese enter file name!!" <<endl<<"./rd0029Normalize -i <Inputfile> -c <classattribute>"<<endl;
		exit(1);
	}
	//Checking if the class name was found or not
	if(!class_found)
	{
		cout<<"Class attribute not defined\nNormalizing all values"<<endl;
	}
	//Create_data_matrix(first run gets the dimension of the data while the second run parses the data and creates the data matrix,File name to be read)
	create_data_matrix(0,file.c_str());
	//create_outputfile(filename that is to be read,boolean-tells if the class attribute was provided or not,if yes the provided class name)
	create_outputfile(file.c_str(),class_found,class_name);
	return 0; 
}