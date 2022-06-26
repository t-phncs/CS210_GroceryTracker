#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

void displayHistogram() {
	ifstream iFile;
	iFile.open("frequency.dat");
	string itemName;
	int itemQuantity;

	if (!iFile.is_open()) {
		cout << "Unable to open file!" << endl;
	}
	iFile >> itemName >> itemQuantity;
	while (!iFile.eof()) {
		cout << itemName << ": ";
		// for loop to count the number of item frequency in the list
		for (int i = 0; i < itemQuantity; i++) {
			//convert to *, 5 = *****
			cout << "*";
		}
		cout << endl;
		iFile >> itemName >> itemQuantity;
	}
	iFile.close();
}

void displayMenu() {
	int numChoice = 0;
	string itemName;
	do {
		cout << endl;
		cout << "1.Total item frequency from Grocery List" << endl;
		cout << "2.Total item frequency from user input based on Grocery List" << endl;
		cout << "3.Display text-based histogram" << endl;
		cout << "4.Exit" << endl;
		cout << "Please enter (1-4): " << endl;
		cin >> numChoice;
		//validate user input 
		while (numChoice < 1 || numChoice > 4 || isdigit(numChoice) != false) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "Please enter a valid number: " << endl;
			cout << "Please enter (1-4): " << endl;
			cin >> numChoice;
		}
		switch (numChoice) {
		case 1:
			cout << "You choose total item frequency from Grocery List" << endl;
			CallProcedure("items_frequency");
			break;
		case 2: 
			cout << "You choose total item frequency from user input based on Grocery List" << endl;
			cout << "Enter an item name: " << endl;
			cin >> itemName;
			//validate user input 
			while (!cin) {
				cin.clear();
				cin.ignore(INT_MAX, 'n');
				cout << "Please enter a valid answer: ";
				cin >> itemName;
			}
			cout << callIntFunc("items_frequency_from_user_input", itemName);
			break;
		case 3:
			cout << "You choose to diisplay text-based histogram" << endl;
			CallProcedure("write_frequency");
			displayHistogram();
			break;
		case 4:
			cout << "You choose exit!" << endl;
			cout << "Exit the program!!!" << endl;
			exit(0);
		}
	} while (numChoice <= 3 && numChoice >= 1);
	
}


int main()
{
	displayMenu();

	return 0;

}