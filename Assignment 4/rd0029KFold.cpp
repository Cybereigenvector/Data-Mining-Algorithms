/////////////////////////////////////////////////////////////////////////////////
//This program parses the arff file and trains a Naive Baeyes classifier. 	   //
//To validiate the trained model K-Fold Cross Validiationg is used			   //
//@Rishabh Das (rd0029) 													   //
//April 2017
/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include <time.h>
#include <cstring>
#include <sstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//The data_matrix and its dimensions are declared globally.                   //
////////////////////////////////////////////////////////////////////////////////
vector < vector<float> > data_matrix;
vector <string> attribute;
vector < vector<float> > attribute_values;
vector <float> class_number;                        //Number of data per class 
vector <vector<vector <float> > > clss_att_num;		//3D Vector with the probabilities	
vector <float> predicted_class;						//Predicted class
vector <float> k_fold_population;					//Number of values in each folds
vector <vector<vector <float> > > each_fold;			//3D Vector having K folds of data vectors
vector <float> index_vector;						//Since the indices of the data matrix are getting randomly re-shuffled this vector stores the original indices
vector <vector <float> > fold_index_2D;				//Stores the indices of the data being assigned to the fold
vector <vector <float> > confusion_matrix;
vector <float> accuracy_matrix;

int column,row,num_attribute,clss_index,control;
float prev_ratio;

////////////////////////////////////////////////////////////////////////////////
//This function creates the data matrix and the attribute matrix 			  //	
//The matrix is implemented as a C++ vector. After the function is 		      //	
//invoked this function parses the .arff file and stroes the data 		      //	
//to the data_matrix the name of the attributes and the class is              //	
//stored in the attribute vector. when run=0 the function stores the 	      //
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
		 	int attribute_num=0;
		 	vector <float> temp_vector;

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

   		 			position=line.rfind("{");
   		 			
   		 			while(line.at(position)!='}' && position < length)
   		 			{	
   		 				position++;
   		 				while(line.at(position)==' ')
   		 				{
   		 					position++;
   		 				}
   		 				while(line.at(position)!=',' && line.at(position)!='}')
		 				{
		 					att+=line.at(position);
		 					position++;
		 				}
		 				attribute_values[num_attribute-1][attribute_num]=atof(att.c_str());
		 				att="";
		 				attribute_num++;


   		 			}
   		 			attribute_num=0;
		 			
   		 		}
   		 		else if(run==0)
   		 		{
   		 			while(line.at(position)!='}')
   		 			{
   		 				if(line.at(position)==',')
   		 				{
   		 					attribute_num++;
   		 				}
   		 				position++;
   		 			}
   		 			attribute_num++;
   		 			temp_vector.resize(attribute_num);
   		 			attribute_values.push_back(temp_vector);
   		 			attribute_num=0;
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
    	index_vector.resize(data_matrix.size());
		predicted_class.resize(data_matrix.size());
    	//cout<<endl<<"The data read from file is as follows:-"<<endl;
    	for(int i=0;i<data_matrix.size();i++)
    	{
    		/*for(int j=0;j<data_matrix[i].size();j++)
    		{
    			cout <<data_matrix[i][j]<<"  ";
    		}*/
    		index_vector[i]=i;
    		//cout<<index_vector[i]<<endl;
    	//	cout<<endl;
    	}
    	/*cout<<endl<<"The data matrix was generated!!"<<endl;
    	cout<<endl<<"The Attribute matrix is generated!"<<endl;

    	for(int i=0;i<attribute_values.size();i++)
    	{
    		for(int j=0;j<attribute_values[i].size();j++)
    		{
    			cout <<attribute_values[i][j]<<"  ";
    		}
    		cout<<endl;
    	}
    	cout<<"The total number of samples "<<row<<endl;*/

    	

	}
}
//////////////////////////////////////////////////////////////////////////////////////
//This function is finding the class_index.This index will be used in the program to//
//understand the location of the class index in the data matrix. A vector having the//
//names of the attribute is taken as an argument of the function				    //
////////////////////////////////////////////////////////////////////////////////////// 
void find_class_clss_index(bool class_present,string clss_name,vector <string> att_nam)
{
	if(class_present)
	{
		for(clss_index=0;clss_index<att_nam.size();clss_index++)
		{
			if(clss_name.compare(att_nam[clss_index])==0)
			{
				break;
			}
		}
	}
	else
	{
		clss_index=att_nam.size()+10;
	}

	cout<<"The class index is "<<clss_index<<endl;
}
//////////////////////////////////////////////////////////////////////////////////////
//First it counts the number of class value in the given  distribution.This function// 
//prepares the 3D vecrtor that stores all the counted values.This is the function   //
//that calculates the required probabilities 									    //
//////////////////////////////////////////////////////////////////////////////////////
void count_class(vector <vector <float> >data)
{
	vector <float> temp_vector_interior;
	vector <vector <float> > temp_final;

	class_number.clear();
	class_number.resize(attribute_values[clss_index].size());

	for(int i=0;i<attribute_values[clss_index].size();i++)
	{
		for(int j=0;j<data.size();j++)
		{
			if(data[j][clss_index]==attribute_values[clss_index][i])
			{
				class_number[i]++;
			}
		}
	}
	//Preparing te 3D probability vector
	clss_att_num.clear();
	temp_vector_interior.resize(attribute_values[clss_index].size());
	for(int i=0;i<attribute_values.size();i++)
	{
		
		for(int j=0;j<attribute_values[i].size();j++)
		{
			temp_final.push_back(temp_vector_interior);
		}
		clss_att_num.push_back(temp_final);
		temp_final.clear();
	}
	
	for(int i=0;i<attribute_values.size();i++)
	{
		if(i!=clss_index)
		{
			for(int j=0;j<attribute_values[clss_index].size();j++)
			{
				for(int k=0;k<attribute_values[i].size();k++)
				{
					for(int l=0;l<data.size();l++)
					{
						if(data[l][clss_index]==attribute_values[clss_index][j] && data[l][i]==attribute_values[i][k])
						{
							clss_att_num[i][k][j]++;
						}			
					}
				}
			}
		}
	}
	for(int i=0;i<attribute_values.size();i++)
	{
		if(i!=clss_index)
		{
			for(int j=0;j<attribute_values[clss_index].size();j++)
			{
				for(int k=0;k<attribute_values[i].size();k++)
				{
					clss_att_num[i][k][j]=clss_att_num[i][k][j]/class_number[j];		
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////
//This function segments the data into folds.This is stored in a 3D 	 //
//vector.In this way it can be used to pass segments of the data to the  //
//actual count class function 											 //	
///////////////////////////////////////////////////////////////////////////
void create_3D_fold_vector(vector <vector<float> > data,int k_fold)
{	
	int clipped_index,loop;
	vector <vector <float> > temp_vector;
	
	loop=0;

	while(loop<k_fold)
	{
		for(int i=0;i<k_fold_population[loop];i++)
		{
			srand(time(NULL));
			clipped_index=rand()%data.size();
			temp_vector.push_back(data[clipped_index]);
			data.erase(data.begin()+clipped_index);
			fold_index_2D[loop][i]=index_vector[clipped_index];
			index_vector.erase(index_vector.begin()+clipped_index);
		}
		each_fold.push_back(temp_vector);
		temp_vector.clear();
		loop++;
	} 
}
////////////////////////////////////////////////////////////////////////////
//This functions decides how many data indices are to be designated to 	  //
//each fold This function also creates the index matix for holding the    //
//randomized indices. 													  //
////////////////////////////////////////////////////////////////////////////
void create_folds_population(int k_fold,int size)
{
	int total_sum;
	vector <float> temp_vec;

	total_sum=0;
	k_fold_population.resize(k_fold);
													
	while(total_sum<size)
	{
		for(int i=0;i<k_fold;i++)
		{
			if(total_sum<size)
			{
				k_fold_population[i]++;
				total_sum++;
			}

		}
	}
	for(int i=0;i<k_fold;i++)
	{
		temp_vec.resize(k_fold_population[i]);
		fold_index_2D.push_back(temp_vec);
		temp_vec.clear();
	}
}
//////////////////////////////////////////////////////////////////////////////////////
//Helper function.Returns the Index of the attriubute value being matched while 	//	
//testing 																			//
//////////////////////////////////////////////////////////////////////////////////////
int get_index(int att_number, int att_value)
{
	for(int i=0;i<attribute_values[att_number].size();i++)
	{
		if(att_value==attribute_values[att_number][i])
		{
			return i;
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//This function tests the trained model and come up with the predicted value 		//
//////////////////////////////////////////////////////////////////////////////////////
void test_trained_model(vector<vector <float> >data,int test_fold,int sum)
{
	float probability,prediction,clss_value;
	float accuracy;

	accuracy=0;
	prediction=0.0;
	clss_value=-1;
	for(int i=0;i<data.size();i++)
	{
		for(int j=0;j<attribute_values[clss_index].size();j++)
		{
			probability=1.0;
			for(int k=0;k<data[i].size();k++)
			{
				if(k!=clss_index)
				{
					probability=probability*clss_att_num[k][get_index(k,data[i][k])][j];
				
				}
			}
			probability=probability*(class_number[j]/sum);

			if(probability>prediction || j==0)
			{
				prediction=probability;
				clss_value=attribute_values[clss_index][j];
			}
			
		}
		prediction=0.0;
		predicted_class[fold_index_2D[test_fold][i]]=clss_value;
		if(data_matrix[fold_index_2D[test_fold][i]][clss_index]==predicted_class[fold_index_2D[test_fold][i]])
		{
			accuracy++;
		}
	}
	accuracy=accuracy/fold_index_2D[test_fold].size();
	accuracy_matrix[test_fold]=accuracy;
	cout<<"Accuracy of test fold "<<test_fold+1<<" is "<<accuracy<<endl;
}
//////////////////////////////////////////////////////////////////////////////////////
//This function merges the K folds and trains the classifier using that. Then the 	//
//trained model is tested by callinig the necessary functions   					//
//////////////////////////////////////////////////////////////////////////////////////
void K_Fold_Crossvalidiation(int k_fold)
{
	vector<vector <float> > data_temp_vec;

	for(int i=0;i<k_fold;i++)
	{
		for(int j=0;j<k_fold;j++)
		{
			if(j!=i)
			{
				for(int k=0;k<each_fold[i].size();k++)
				{
					data_temp_vec.push_back(each_fold[i][k]);
				}
			}
		}
		count_class(data_temp_vec);
		test_trained_model(each_fold[i],i,data_temp_vec.size());
		data_temp_vec.clear();
		
	}
}
//////////////////////////////////////////////////////////////////////////////////////
//This function creates the confusion matrix,calculates the accuracy and outputs the//
//result to a txt file 																//
//////////////////////////////////////////////////////////////////////////////////////
void compute_confusion_matrix(string fold_val,string filename)
{
	vector<float> temp_vector;
	string outputfilename,line;
	float accuracy;
	bool done;
	int count_att=0;
	ifstream myfile (filename.c_str());

	accuracy=0;
	outputfilename="";
	outputfilename="rd0029"+outputfilename+fold_val.c_str()+"FoldConfusion"+filename.c_str();

	ofstream outfile_confusion(outputfilename.c_str());
	outfile_confusion.is_open();

	accuracy=0;
	temp_vector.resize(attribute_values[clss_index].size());

	for(int i=0;i<attribute_values[clss_index].size();i++)
	{
		confusion_matrix.push_back(temp_vector);
	}
	for(int i=0;i<data_matrix.size();i++)
	{
		for(int j=0;j<confusion_matrix.size();j++)
		{
			for(int k=0;k<confusion_matrix[0].size();k++)
			{
				if(attribute_values[clss_index][j]==data_matrix[i][clss_index] && predicted_class[i]==attribute_values[clss_index][k])
				{
					confusion_matrix[j][k]++;
				}
			}
		}
	}
	for(int i=0;i<accuracy_matrix.size();i++)
	{
		outfile_confusion<<"The accuracy of fold "<<i+1<<" is "<<accuracy_matrix[i]*100<<"%"<<endl;
	}
	for(int i=0;i<attribute_values[clss_index].size()+1;i++)
	{	
		outfile_confusion<<"------------";
	}
	outfile_confusion<<endl;
	outfile_confusion<<setw(15)<<"\t\tPredicted Class";
	outfile_confusion<<endl;
	for(int i=0;i<attribute_values[clss_index].size()+1;i++)
	{	
		outfile_confusion<<"------------";
	}
	outfile_confusion<<endl;
	outfile_confusion<<"\t\t\t\t\t";
	outfile_confusion<<"|";
	for(int i=0;i<attribute_values[clss_index].size();i++)
	{
		outfile_confusion<<setw(5)<<attribute_values[clss_index][i]<<"\t|";
	}
	outfile_confusion<<endl;
	outfile_confusion<<"\t\t\t\t\t ";
	for(int i=0;i<attribute_values[clss_index].size()+1;i++)
	{	
		outfile_confusion<<"-----";
	}
	outfile_confusion<<endl;
	outfile_confusion<<"Actual Value";
	for(int i=0;i<confusion_matrix.size();i++)
	{
		if(i!=0)
		{
			outfile_confusion<<"            ";
		}
		for(int j=0;j<confusion_matrix[0].size();j++)
		{
			if(i==j)
			{
				accuracy=accuracy+confusion_matrix[i][j];
			}
			if(j==0)
			{
				outfile_confusion<<setw(5)<<attribute_values[clss_index][i]<<"\t|";
			}
			outfile_confusion<<setw(5)<<confusion_matrix[i][j]<<"\t|";
		}
		outfile_confusion<<endl;
	}
	outfile_confusion<<endl;

	outfile_confusion<<"The Overall accuracy is ";
	outfile_confusion<<(accuracy/data_matrix.size())*100;
	outfile_confusion<<" %";
	outfile_confusion.close();

	outputfilename="";
	outputfilename="rd0029"+outputfilename+fold_val.c_str()+"FoldClassification"+filename.c_str();
	ofstream outfile_att(outputfilename.c_str());
	outfile_att.is_open();
	done=true;

	if (myfile.is_open())
	{
		 while (getline(myfile,line) && done)
		 {
		 	//find function is used to detect the word "data" and "attributes"
		 	int found_relation=line.find("relation");
		 	int found_attribute=line.find("attribute");
		 	if(found_relation!=std::string::npos)
		 	{
		 		outfile_att<<line;
		 	}
		 	else if(count_att<num_attribute+1)
		 	{
		 		count_att++;
		 		outfile_att<<line;
		 	}
		 	else
		 	{	
		 		
		 		outfile_att<<"@attribute bayesClass real {";
		 		for(int i=0;i<attribute_values[clss_index].size();i++)
		 		{
		 			outfile_att<<attribute_values[clss_index][i];
		 			if(i+1==attribute_values[clss_index].size())
		 			{
		 				outfile_att<<"}";
		 				outfile_att<<endl;
		 				outfile_att<<endl;
		 				outfile_att<<"@data";
		 				outfile_att<<endl;
		 			}
		 			else
		 			{
		 				outfile_att<<" ,";
		 			}	
		 		}
		 		for(int i=0;i<data_matrix.size();i++)
		 		{
		 			for(int j=0;j<data_matrix[0].size()+1;j++)
		 			{
		 				

		 				if(j==data_matrix[0].size())
		 				{
		 					outfile_att<<setw(5)<<predicted_class[i];
		 				}
		 				else
		 				{
		 					outfile_att<<setw(5)<<data_matrix[i][j];
		 				}
		 			}
		 			outfile_att<<endl;
		 		}
		 		done=false; 

		 	}
		 }

	}
	else
	{
		cout<<"Error Openning file!!"<<endl;
	}
	myfile.close();
	outfile_att.close();

}
///////////////////////////////////////////////////////////////////////////////////////
//This is the main function. The commandline arguments are parsed and  handled in    //
//this function. After the required arguments are provided by the user this 	     //
//function calls the required function to perform the required operation  		   	 //
///////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	string file,class_name,K,name;
	bool file_found,class_found,k_found;;
	vector<float> temp_vec;
	vector <string> command_line;


	file_found=false;
	class_found=false;
	k_found=false;

///////////////////////////////////////////////////////////////////////////////////////
//The section of the main fucntion parses the commandline input and assigns the      //
//values to appropriate variable												     //
///////////////////////////////////////////////////////////////////////////////////////

	class_name="";
	K="";

	if (argc > 7 || argc < 5)
	{
		cout <<"./rd0029 -i <Inputfile> -c <classattribute> -K K"<<endl;
		exit(1);
	}
	//storing the arv values to a string vector
	command_line.assign(argv+1,argv+argc);
	
	//parsing andchecking the command line arguments
	for(int i=0;i<command_line.size()-1;i++)
	{
		if(command_line[i].compare("-i")==0 && i+1 <command_line.size ())
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
	}
	//Checking if the file name was found or not
	if(!file_found || !k_found)
	{
		cout<<"Plese check the inputs!!" <<endl<<"./rd0029 -i <Inputfile> -c <classattribute> -K K"<<endl;
		exit(1);
	}

	cout<<"The file name is->"<<file<<endl<<"Class attribute->"<<class_name<<endl<<"Number of folds being created is->"<<K<<endl;
	accuracy_matrix.resize(atoi(K.c_str()));
	
///////////////////////////////////////////////////////////////////////////////////////
//This segment of the main function is used to call all the required functions to    //
//perform the required functionality                                                 //
///////////////////////////////////////////////////////////////////////////////////////

	
	//Create_data_matrix(first run gets the dimension of the data while the second run parses the data and creates the data matrix,File name to be read)
	create_data_matrix(0,file.c_str());
	if(atoi(K.c_str())<2 || atoi(K.c_str())>data_matrix.size())
	{
		cout<<"Plese check the inputs!!\nUnacceptable K value!\n";
		exit(1);
	}
	find_class_clss_index(class_found,class_name,attribute);
	create_folds_population(atoi(K.c_str()),data_matrix.size());
	create_3D_fold_vector(data_matrix,atoi(K.c_str()));
	K_Fold_Crossvalidiation(atoi(K.c_str()));
	compute_confusion_matrix(K.c_str(),file.c_str());

	float acc=0;
	for(int i=0;i<data_matrix.size();i++)
	{
		if(data_matrix[i][clss_index]==predicted_class[i])
		{
			acc++;
		}
	}
	cout<<"Overall Accuracy->"<<(acc/data_matrix.size());
	
	return 0; 
}
