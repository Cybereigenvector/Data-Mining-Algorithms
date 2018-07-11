/////////////////////////////////////////////////////////////////////////////////
//This program parses the arff file and trains a decision tree. The percentage //
//of data used for traing is provided by the user as commandline input. Finally//
//the trained tree is validiated by 100% of the data provided as input         //
//@Rishabh Das (rd0029) 													   //
//Date:-31st March 2017														   //
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
vector< vector<float> > data_matrix;
vector<string> attribute;
vector< vector<float> > attribute_values;
vector<float> gain_vector;
vector<vector <float> > calculation_matrix;
vector<float> decision_vector;
vector<vector <float> > confusion_matrix;

int column,row,num_attribute,clss_index;
float accuracy;

////////////////////////////////////////////////////////////////////////////////
//Basic tree node structure that represents each node of the tree 			  //
////////////////////////////////////////////////////////////////////////////////
struct tree_node
{
	bool ROOT_NODE;
	bool CLASSIFIED;
	string ATTRIBUTE_NAME;
	float TREE_LEVEL;
	float NODE_DECISION_LABEL;
	float ENTROPY_NODE;
	tree_node * PREV_NODE;

	vector <float> DATA_POINTS;
	vector <struct tree_node *> CHILDREN;
	vector <float> CHILDREN_VALUES;
};
////////////////////////////////////////////////////////////////////////////////
//Storing the starting point of the tree data structure. The ROOT node of the //
//tree is the starting point.It is set to NULL at the start of the program	  //
////////////////////////////////////////////////////////////////////////////////
struct tree_node *ROOT=NULL;


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
    	/*cout<<endl<<"The data read from file is as follows:-"<<endl;
    	for(int i=0;i<data_matrix.size();i++)
    	{
    		for(int j=0;j<data_matrix[i].size();j++)
    		{
    			cout <<data_matrix[i][j]<<"  ";
    		}
    		cout<<endl;
    	}
    	cout<<endl<<"The data matrix was generated!!"<<endl;
    	cout<<endl<<"The Attribute matrix is generated!"<<endl;

    	for(int i=0;i<attribute_values.size();i++)
    	{
    		for(int j=0;j<attribute_values[i].size();j++)
    		{
    			cout <<attribute_values[i][j]<<"  ";
    		}
    		cout<<endl;
    	}
    	cout<<"The total number of samples "<<row<<endl;
*/
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
}
//////////////////////////////////////////////////////////////////////////////////////
//This function returns the entropy of a single attribute.The index of the attribute//
//the pointer to the current node, the trimmed data matrix, a vector of vectors 	//
//containing the possible float values of the attributes and the attribute names are//
//taken as arguments in the function 												//
//////////////////////////////////////////////////////////////////////////////////////
float single_attribute_entropy(int attribute_clss_index,struct tree_node *CURR,vector<vector <float> > dt,vector<vector<float> > a_val,vector<string> a_nam)
{
	//This part of the function makes the attribute matrix that is necessary for the calculation of entropy
	vector <float> temp_vector;
	float total,entropy,temp;

	total=0;
	entropy=0;
	temp=0;
	temp_vector.resize(a_val[clss_index].size()+2);
	calculation_matrix.clear();

	for(int i=0;i<a_val[attribute_clss_index].size();i++)
	{
		calculation_matrix.push_back(temp_vector);
	}

	//This part of the program calculates the parameters in the matrix
	for(int i=0;i<a_val[attribute_clss_index].size();i++)
	{
		for(int j=0;j<a_val[clss_index].size();j++)
		{
			for(int k=0;k<CURR->DATA_POINTS.size();k++)
			{
				if (dt[CURR->DATA_POINTS[k]][attribute_clss_index]==a_val[attribute_clss_index][i] && dt[CURR->DATA_POINTS[k]][clss_index]==a_val[clss_index][j])
				{
					calculation_matrix[i][j]++;
				}	
			}
			total=total+calculation_matrix[i][j];
		}
		calculation_matrix[i][calculation_matrix[i].size()-2]=total;
		total=0;
	}
	total=0;
	for(int i=0;i<calculation_matrix.size();i++)
	{
		for(int j=0;j<calculation_matrix[i].size();j++)
		{
			if(j<calculation_matrix[i].size()-2)
			{
				if(calculation_matrix[i][j]==0)
				{
					temp=0;
				}
				else
				{
					temp=(calculation_matrix[i][j]/calculation_matrix[i][calculation_matrix[i].size()-2])*log2(calculation_matrix[i][j]/calculation_matrix[i][calculation_matrix[i].size()-2]);
				}
				total=total-temp;
				temp=0;
			}
			else if(j==calculation_matrix[i].size()-1)
			{
				calculation_matrix[i][calculation_matrix[i].size()-1]=total*(calculation_matrix[i][calculation_matrix[i].size()-2]/CURR->DATA_POINTS.size());
				entropy=entropy+calculation_matrix[i][calculation_matrix[i].size()-1];
			}
		}
		total=0;
	} 
	return entropy;
}
//////////////////////////////////////////////////////////////////////////////////////
//This function preapres a vector containing all the Information Gain values. This  //
//function calls the single entropy function to get all the entropy values for all  //
//the attributes.These values are stored in a vector. 								//
//////////////////////////////////////////////////////////////////////////////////////
void prepare_gain_vector(struct tree_node *CURRENT,vector<vector <float> > dat,vector<vector<float> > at_val,vector<string> at_nam )
{
	gain_vector.clear();
	gain_vector.resize(dat[0].size());
	
	for(int x=0;x<dat[0].size();x++)
	{
		if(x!=clss_index)
		{
			gain_vector[x]=single_attribute_entropy(x,CURRENT,dat,at_val,at_nam);
		}
		else
		{
			gain_vector[x]=1000;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////
//This function scans through the gain vector and selects and returns the attribute //
//with lowest entropy 																//
//////////////////////////////////////////////////////////////////////////////////////
int lowest_entropy()
{
	float lowest,lowest_clss_index;
	lowest=gain_vector[0];
	lowest_clss_index=0;
	for(int i=0;i<gain_vector.size();i++)
	{
		if(lowest>gain_vector[i])
		{
			lowest=gain_vector[i];
			lowest_clss_index=i;
		}
	}
	return lowest_clss_index;
}
//////////////////////////////////////////////////////////////////////////////////////
//This is the function that takes care of all the data once the split attribute has //
//been decided upon. This function splits the datatset into nodes and copies what  	// 	
//data is to be held by each node. Actually the data is not copied bu the indices of//
//the data row from the datamatrix is propagated 									//  
//////////////////////////////////////////////////////////////////////////////////////
void propagate_data_to_children(int split_clss_index,float attribute_val,int child_num,struct tree_node *CURRENT,vector<vector <float> > dat)
{
	struct tree_node *PREV;
	PREV=CURRENT;
	CURRENT=CURRENT->CHILDREN[child_num];
	//traverse to the Child node
	for(int j=0;j<PREV->DATA_POINTS.size();j++)
	{
		if(dat[PREV->DATA_POINTS[j]][split_clss_index]==attribute_val)
		{	
			CURRENT->DATA_POINTS.push_back(PREV->DATA_POINTS[j]);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////
//This function trims the attribute that was used previously to split. This is done //
//both from the data matrix and the attribute matrix. Hence allowing the recursive  //
//call to use a trimmed version of the matrix every single time.  					//
//////////////////////////////////////////////////////////////////////////////////////
void trim_attribute(int split_clss_index,vector<vector <float> > &dat,vector<vector<float> > &at_val,vector<string> &at_nam )
{
	int length;
	
	length=at_val[split_clss_index].size();
	
	//cout<<"Number of attribute before trimming"<<at_val.size()<<"  "<<dat[0].size()<<endl;
	for(int j=0;j<dat.size();j++)
	{
		dat[j].erase(dat[j].begin()+split_clss_index);
	}
	at_val.erase(at_val.begin()+split_clss_index);
	at_nam.erase(at_nam.begin()+split_clss_index);
}
//////////////////////////////////////////////////////////////////////////////////////
//This function does majority polling and returns the class value. This function 	//
//takes the pointer to the node whose polling is to be done and the data matrix and //
//the attribute vector as the argument 												//
//////////////////////////////////////////////////////////////////////////////////////
float majority_polling(struct tree_node *CURRENT,int class_size, vector< vector <float> >att,vector< vector <float> >dat)
{
	vector<float>temp_vector;
	int comp,lowest_ind;

	comp=0;
	lowest_ind=0;

	temp_vector.resize(class_size);

	for(int j=0;j<calculation_matrix.size();j++)
	{
		for(int k=0;k<calculation_matrix[0].size()-2;k++)
		{
			temp_vector[k]=temp_vector[k] + calculation_matrix[j][k];
		}
	}
	comp=temp_vector[0];
	for(int j=0;j<temp_vector.size();j++)
	{
		if(comp<temp_vector[j])
		{
			comp=temp_vector[j];
			lowest_ind=j;
		}
	}
	return att[clss_index][lowest_ind];
}
//////////////////////////////////////////////////////////////////////////////////////
//This functions generates the test results after the tree has been created. The  	//
//testing is done using 100% data. this function takes the data matrix row index as //
//the argument.Hence once the roiw is provided the function get a vector of 		//
//attribute values from the data matrix. Finally the values of the attributes in the//
//vector are compared and the tree is traversed accordingly.						//
//////////////////////////////////////////////////////////////////////////////////////
float test_tree(int ii)
{
	int j;
	struct tree_node *CURRENT_NODE; 

	CURRENT_NODE=ROOT;
	while(!CURRENT_NODE->CLASSIFIED)
	{
		for(j=0;j<attribute.size();j++)
		{
			if(attribute[j].compare(CURRENT_NODE->ATTRIBUTE_NAME)==0)
			{
				break;
			}
		}
		for(int k=0;k<CURRENT_NODE->CHILDREN_VALUES.size();k++)
		{
			if(data_matrix[ii][j]==attribute_values[j][k])
			{
				CURRENT_NODE=CURRENT_NODE->CHILDREN[k];
				break;
			}
		}
		if(CURRENT_NODE->CLASSIFIED)
		{
			if(data_matrix[ii][clss_index]==CURRENT_NODE->NODE_DECISION_LABEL)
			{
				accuracy++;
			}
			return CURRENT_NODE->NODE_DECISION_LABEL;
		}	
	}
}
//////////////////////////////////////////////////////////////////////////////////////
//This function handles the entropy calculation of all the attributes and is  		//
//recursively called to implement the decision tree. This is the main function that //
//trains the decision tree and prints the result onto a txt file. The whole training//
//process is acheived by call the other functions recursively. 						//
//////////////////////////////////////////////////////////////////////////////////////
void attribute_entropy(struct tree_node *CURRENT_NODE,bool class_fd,string class_nm,vector<vector <float> > data,vector< vector <float> > att_val,vector<string> att_name,int tree_level,ofstream &outfile_accu)
{
	float low;
	vector< vector<float> > data_matrix_copy;
	vector< vector<float> > attribute_values_copy;
	vector<string> attribute_names_copy;
	struct tree_node *PREVIOUS_NODE;
	

	find_class_clss_index(class_fd,class_nm,att_name);
	prepare_gain_vector(CURRENT_NODE,data,att_val,att_name);
	low=lowest_entropy();
	CURRENT_NODE->CHILDREN.resize(att_val[low].size());
	CURRENT_NODE->ATTRIBUTE_NAME=att_name[low];
	CURRENT_NODE->CHILDREN_VALUES.resize(att_val[low].size());
	PREVIOUS_NODE=CURRENT_NODE;

	if(!CURRENT_NODE->ROOT_NODE)
	{
		tree_level++;
		CURRENT_NODE->TREE_LEVEL=tree_level;
	}

	for(int i=0;i<att_val[low].size();i++)
	{
		CURRENT_NODE->CHILDREN_VALUES[i]=att_val[low][i];
		CURRENT_NODE->CHILDREN[i]=new struct tree_node;
		propagate_data_to_children(low,att_val[low][i],i,CURRENT_NODE,data);
		find_class_clss_index(class_fd,class_nm,att_name);
		CURRENT_NODE->ENTROPY_NODE=single_attribute_entropy(low,CURRENT_NODE,data,att_val,att_name);
		CURRENT_NODE->NODE_DECISION_LABEL=majority_polling(CURRENT_NODE,att_val[clss_index].size(),att_val,data);
	
		if(CURRENT_NODE->DATA_POINTS.size()!=0)
		{

			if(calculation_matrix[i][calculation_matrix[i].size()-1]!=0 )
			{
				if(data[0].size()>2)
				{
					data_matrix_copy.clear();
					attribute_values_copy.clear();
					attribute_names_copy.clear();

					data_matrix_copy=data;
					attribute_values_copy=att_val;
					attribute_names_copy=att_name;

					trim_attribute(low, data,att_val,att_name);
					find_class_clss_index(class_fd,class_nm,att_name);
					CURRENT_NODE->CHILDREN[i]->CLASSIFIED=false;

					for(int j=0;j<tree_level;j++)
					{
						cout<<" | ";
						outfile_accu<<" | ";
					}
					cout<<CURRENT_NODE->ATTRIBUTE_NAME<<" = "<<att_val[low][i];
					outfile_accu<<CURRENT_NODE->ATTRIBUTE_NAME<<" = "<<att_val[low][i]<<endl;
					cout<<endl;

					if(!CURRENT_NODE->ROOT_NODE)
					{
						CURRENT_NODE->CHILDREN[i]->PREV_NODE=CURRENT_NODE;
					}

					attribute_entropy(CURRENT_NODE->CHILDREN[i],class_fd,class_nm, data,att_val,att_name,tree_level,outfile_accu);

					data.clear();
					att_val.clear();
					att_name.clear();

					data=data_matrix_copy;
					att_val=attribute_values_copy;
					att_name=attribute_names_copy;
					find_class_clss_index(class_fd,class_nm,att_name);
				}
				else
				{
					for(int j=0;j<tree_level;j++)
					{
						cout<<" | ";
						outfile_accu<<" | ";
					}
					cout<<att_name[low]<<" = "<<att_val[low][i]<<" "<<PREVIOUS_NODE->NODE_DECISION_LABEL<<endl;
					outfile_accu<<att_name[low]<<" = "<<att_val[low][i]<<" "<<PREVIOUS_NODE->NODE_DECISION_LABEL<<endl;
					CURRENT_NODE->NODE_DECISION_LABEL=PREVIOUS_NODE->NODE_DECISION_LABEL;
					CURRENT_NODE->CLASSIFIED=true;
				}
			}
			else if(calculation_matrix[i][calculation_matrix[i].size()-1]==0)
			{
				for(int j=0;j<tree_level;j++)
				{
					cout<<" | ";
					outfile_accu<<" | ";
				}
				int j;
				for( j=0;j<calculation_matrix[i].size()-3;j++)
				{
					if(calculation_matrix[i][j]!=0)
					{
						break;
					}
				}

				find_class_clss_index(class_fd,class_nm,att_name);
				stringstream stream_var;
				stream_var<<att_val[clss_index][j];
				cout<<att_name[low]<<" = "<<att_val[low][i]<<" "<<att_val[clss_index][j]<<endl; // all in single class condition
				outfile_accu<<att_name[low]<<" = "<<att_val[low][i]<<" "<<att_val[clss_index][j]<<endl;
				
				CURRENT_NODE->CHILDREN[i]->NODE_DECISION_LABEL=att_val[clss_index][j];
				CURRENT_NODE->CHILDREN[i]->CLASSIFIED=true;

			}
			else 
			{
				for(int j=0;j<tree_level;j++)
				{
					cout<<" | ";
					outfile_accu<<" | ";
				}
				
				find_class_clss_index(class_fd,class_nm,att_name);
				stringstream stream_var;
				stream_var<<data[CURRENT_NODE->DATA_POINTS[0]][clss_index];
				cout<<att_name[low]<<" = "<<att_val[low][i]<<" "<<stream_var.str()<<endl;
				outfile_accu<<att_name[low]<<" = "<<att_val[low][i]<<" "<<stream_var.str()<<endl;
				CURRENT_NODE->CHILDREN[i]->NODE_DECISION_LABEL=data[CURRENT_NODE->DATA_POINTS[0]][clss_index];
				
				CURRENT_NODE->CHILDREN[i]->CLASSIFIED=true;
			}
		}
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////
//This function creates the confusion matrix,calculates the accuracy and outputs the//
//result to a txt file 																//
//////////////////////////////////////////////////////////////////////////////////////
void compute_confusion_matrix(string percent,string filename)
{
	vector<float> temp_vector;
	string outputfilename;
	outputfilename="";
	outputfilename="rd0029jro0005DTAccuracy"+outputfilename+percent.c_str()+filename.c_str();

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
				if(attribute_values[clss_index][j]==data_matrix[i][clss_index] && decision_vector[i]==attribute_values[clss_index][k])
				{
					confusion_matrix[j][k]++;
				}
			}
		}
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
	outfile_confusion<<"The accuracy is ";
	outfile_confusion<<(accuracy/data_matrix.size())*100;
	outfile_confusion<<" %";
	outfile_confusion.close();

}
//////////////////////////////////////////////////////////////////////////////////////
//This function is responsible for creating the output files 						//
//////////////////////////////////////////////////////////////////////////////////////
void create_outputfiles(string percent,string filename)
{
	string line,output1;
	bool done;
	int count_att=0;
	ifstream myfile (filename.c_str());

	output1="";
	
	output1="rd0029jro0005DTApply";
	output1=output1+percent.c_str()+filename.c_str();

	ofstream outfile_att(output1.c_str());

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
		 		
		 		outfile_att<<"@attribute dt_class real {";
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
		 					outfile_att<<setw(5)<<decision_vector[i];
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
//////////////////////////////////////////////////////////////////////////////////////
//This function initializes ths root node of the tree 								//	
//////////////////////////////////////////////////////////////////////////////////////
void initialize_root_node()
{
	ROOT=new struct tree_node;
	ROOT->ROOT_NODE=true;
	ROOT->TREE_LEVEL=0;
	ROOT->DATA_POINTS.resize(row);
	ROOT->CLASSIFIED=false;
	for(int i=0;i<ROOT->DATA_POINTS.size();i++)
	{
		ROOT->DATA_POINTS[i]=i;
	}
}
///////////////////////////////////////////////////////////////////////////////////////
//This is the main function. The commandline arguments are parsed and  handled in    //
//this function. After the required arguments are provided by the user this 	     //
//function calls the required function to perform the required operation  		   	 //
///////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	string file,class_name,T,name;
	bool file_found,class_found,t_found;
	float clipped;
	vector<vector <float> > clipped_data_matrix;
	vector<float> temp_vec;
	vector <string> command_line;


	file_found=false;
	class_found=false;
	t_found=false;
	accuracy=0;

///////////////////////////////////////////////////////////////////////////////////////
//The section of the main fucntion parses the commandline input and assigns the      //
//values to appropriate variable												     //
///////////////////////////////////////////////////////////////////////////////////////

	class_name="";
	T="";

	if (argc > 7 || argc < 5)
	{
		cout <<"./rd0029jro0005DT -i <Inputfile> -c <classattribute> -T M"<<endl;
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
		else if (command_line[i].compare("-T")==0 && i+1 <command_line.size())
		{
			T=command_line[i+1];
			clipped=atof(T.c_str());
			t_found=true;
		}	
	}
	//Checking if the file name was found or not
	if(!file_found || !t_found || clipped>100)
	{
		cout<<"Plese check the inputs!!" <<endl<<"./rd0029jro0005DT -i <Inputfile> -c <classattribute> -T M"<<endl;
		exit(1);
	}

	cout<<"The file name is->"<<file<<endl<<"Class attribute->"<<class_name<<endl<<"Percentage of data being used->"<<T<<endl;
	name="rd0029jro0005DTTrain"+T+file;
	//This is the file where the trained tree would be outputted. The file poiter is 
	//passed to the recursive function later so that the tree can be outputted while training
	ofstream outfile_ac(name.c_str());
	outfile_ac.is_open();


///////////////////////////////////////////////////////////////////////////////////////
//This segment of the main function is used to call all the required functions to    //
//perform the required functionality                                                 //
///////////////////////////////////////////////////////////////////////////////////////

	//Create_data_matrix(first run gets the dimension of the data while the second run parses the data and creates the data matrix,File name to be read)
	create_data_matrix(0,file.c_str());

	//This function finds the clss_index of the class attribute which has to Skipped during normalization
	find_class_clss_index(class_found,class_name,attribute);

	//The data received from the original file is clipped according to the % and stored in the clipped data matrix
	clipped=(atof(T.c_str())/100)*data_matrix.size();
	clipped=(int)clipped;
	temp_vec.resize(data_matrix[0].size());

	for(int i=0;i<clipped;i++)
	{
		clipped_data_matrix.push_back(temp_vec);
	}
	for(int i=0;i<clipped_data_matrix.size();i++)
	{
		for(int j=0;j<clipped_data_matrix[0].size();j++)
		{
			clipped_data_matrix[i][j]=data_matrix[i][j];
		}
	}
	//Since the data is clipped the value of the row and column is updated
	row=clipped_data_matrix.size();
	column=clipped_data_matrix[0].size();
	cout<<"The data has been clipped,It has "<<clipped_data_matrix.size()<<" data rows"<<endl;
	cout<<"rows->"<<row<<endl;
	cout<<"column->"<<column<<endl;

	//the ROOT node of the tree is initialized by this functiopn
	initialize_root_node();

	//This is the first call to the recursive function.The training is handled completely by this function after this call
	attribute_entropy(ROOT,class_found,class_name,clipped_data_matrix,attribute_values,attribute,0,outfile_ac);
	outfile_ac.close();
	
	//This loop sends a row of vector from the data matrix so that the decision can be made
	decision_vector.resize(data_matrix.size());
	for(int i=0;i<data_matrix.size();i++)
	{
		decision_vector[i]=test_tree(i);
	}

	cout<<"The accuracy after validiation is-> "<<(accuracy/data_matrix.size())*100<<" %"<<endl;
	find_class_clss_index(class_found,class_name,attribute);
	
	//This function creates the required outputs of the program
	create_outputfiles(T.c_str(),file.c_str());
	
	//This function computes the confusion matrix and the accuracy of the testing and finally outputs the result to a file
	compute_confusion_matrix(T.c_str(),file.c_str());
		
	return 0; 
}
