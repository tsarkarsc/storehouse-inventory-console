#include <cstring>
#include <new>
#include <iomanip>
#include "AidApp.h"

using namespace std;

namespace sict
{
    AidApp::AidApp(const char* filename)
    {
        strncpy(filename_, filename, 256);
        for(int i = 0; i<MAX_NO_RECS; i++)
        {
            product_[i] = nullptr;
        }
        noOfProducts_ = 0;
        loadRecs();
    }

    void AidApp::pause() const
    {
        cout << "Press Enter to continue..." << endl;
        cin.ignore();
    }

    int AidApp::menu()
    {

        int selection = 0;

        if(!cin.fail())
        {
            cout << "Disaster Aid Supply Management Program" << endl <<
            "1- List products" << endl <<
            "2- Display product" << endl <<
            "3- Add non-perishable product" << endl <<
            "4- Add perishable product" << endl <<
            "5- Add to quantity of purchased products" << endl <<
            "0- Exit program" << endl << "> ";
            cin >> selection;
        }

        if (selection < 0 || selection > 5)
        {
            selection = -1;
        }

        cin.clear();
        cin.ignore(256, '\n');
        return selection;
    }

    void AidApp::loadRecs()
    {
        int index = 0;
        char tag = '\0';
        datafile_.open(filename_, ios::in);

        if (datafile_.fail())
        {
            datafile_.clear();
            datafile_.close();
            datafile_.open(filename_, ios::out);
            datafile_.close();
        }
        else
        {
            while (index < MAX_NO_RECS && !datafile_.eof())
            {
                if (product_[index])
                { 
                    delete[] product_[index]; 
                    product_[index] = nullptr;
                }

                tag = datafile_.get();

                if (tag == 'P') 
                { 
                    product_[index] = new AmaPerishable();
                    datafile_.ignore(1);
                    product_[index]->load(datafile_);
                    index++;
                }
                else if (tag == 'N') 
                { 
                    product_[index] = new AmaProduct();
                    datafile_.ignore(1);
                    product_[index]->load(datafile_);
                    index++;
                }
            }

            noOfProducts_ = index;
        }

        datafile_.close();
    }

    void AidApp::saveRecs()
    {
        datafile_.open(filename_, ios::out);
        for (int i = 0; i < noOfProducts_; i++) 
        { 
            product_[i]->store(datafile_); 
        }
        datafile_.close();
    }

    void AidApp::listProducts()
    {
        double tCost = 0;

        cout << setfill(' ') << setw(4) << right << "Row" << " | ";
        cout << setw(MAX_SKU_LEN) << left << "SKU" << "| ";
        cout << setw(19) << left << "Product Name" << "| ";
        cout << setw(6) << left << "Cost" << "|";
        cout << setw(4) << right << "QTY" << "| ";
        cout << setw(9) << left << "Unit" << "|";
        cout << setw(4) << right << "Need" << "| ";
        cout << setw(9) << left << "Expiry" << endl;
        //cout << "-----|--------|---------------------|--------|-----|-----------|----|-----------" << endl;
        cout << "-----|--------|--------------------|-------|----|----------|----|----------" << endl;

        for (int i = 0; i < noOfProducts_; i++)
        {
            cout << setw(4) << right << i + 1 << setfill(' ')<< " | ";
            product_[i]->write(cout, true);
            cout << endl;
            tCost += *product_[i];
            if(i==9)
            {
                pause();
            }
        }

        cout << "---------------------------------------------------------------------------" << endl;
        cout << "Total cost of support: " << fixed << setprecision(2) << "$" << tCost << endl;
    }

    int AidApp::SearchProducts(const char* sku) const
    {

        bool foundProd = false;
        int i = 0, index = 0;

        while (!foundProd)
        {
            for (i = 0; i < noOfProducts_; i++)
            {
                if (*product_[i] == sku)
                {
                    foundProd = true;
                    index = i;
                    i = noOfProducts_;
                }
            }
        }

        if (!foundProd) 
        { 
            index = -1; 
        } 
        return index;

    }

    void AidApp::addQty(const char* sku)
    {

        int i = 0, updatedQty = 0;
        i = SearchProducts(sku);
        int quantNeeded;
        int quant;
        int required;
        int extra;

        if (i != -1)
        {
            product_[i]->write(cout, false);
            cout << endl << endl << "Please enter the number of purchased items: ";
            cin >> updatedQty;
            if (cin.fail()) 
            { 
                cout << "Invalid Quantity value!" << endl;
            }
            else
            {
                quantNeeded = product_[i]->qtyNeeded();
                quant = product_[i]->quantity();
                required = quantNeeded - quant;

                if (updatedQty <= required) 
                { 
                    *product_[i] += updatedQty;
                }
                else
                {
                    extra = updatedQty - required;
                    cout << "Too many items; only " << required << " is needed, please return the extra " << extra << " items." << endl;
                    product_[i]->quantity(quantNeeded);
                }
            }

            saveRecs();
            cout << endl << "Updated!" << endl;
            cin.clear();
            cin.ignore(256, '\n');
        }

        else { cout << "Not found!" << endl; }

    }

    void AidApp::addProduct(bool isPerishable)
    {
        if (!isPerishable)
        {
            AmaProduct nonPerProd;
            cin >> nonPerProd;
            if (cin.fail())
            {
                nonPerProd.write(cout, true);
                cout << endl;
                run();
            }
            else
            {
                datafile_.open(filename_, ios::out);

                for (int i = 0; i < noOfProducts_; i++) 
                { 
                    product_[i]->store(datafile_); 
                }

                nonPerProd.store(datafile_);
                datafile_.close();
                cout << "Product Added!" << endl;
            }
        }
        else
        {
            AmaPerishable perProd;
            cin >> perProd;
            if (cin.fail()) 
            { 
                perProd.write(cout, true);
                cout << endl;
                run();
            }
            else
            {
                datafile_.open(filename_, ios::out);     

                for (int i = 0; i < noOfProducts_; i++) 
                { 
                    product_[i]->store(datafile_); 
                }

                perProd.store(datafile_);
                datafile_.close();
                cout << endl << "Product added" << endl;
            }
        }
        
    }

    int AidApp::run()
    {
        int selection = -1;

        while (selection != 0)
        {
            selection = menu();

            if(selection == 0)
            {
                cout << endl << "Goodbye!!" << endl;
            }
            else if(selection == 1)
            {
                cout << endl;
                listProducts();
                cout << endl;
                pause();
            }
            else if(selection == 2)
            {
                char inSKU[MAX_SKU_LEN+1];
                int prodIndex;

                cout << endl;
                cout << "Please enter the SKU: ";
                cin.getline(inSKU, MAX_SKU_LEN, '\n');
                prodIndex = SearchProducts(inSKU);
                cout << endl;
                product_[prodIndex]->write(cout, false);
                cout << endl << endl;
                pause();
            }
            else if(selection == 3)
            {
                cout << endl;
                addProduct(false);
                cout << endl;

                //refresh the products after adding a new one so it will show up in the list
                for(int i=0; i<MAX_NO_RECS; i++)
                {
                    product_[i] = nullptr;
                }
                loadRecs();
            }
            else if(selection == 4)
            {
                cout << endl;
                addProduct(true);
                cout << endl;

                for(int i=0; i<MAX_NO_RECS; i++)
                {
                    product_[i] = nullptr;
                }
                loadRecs();
            }
            else if(selection == 5)
            {
                char inSKU[MAX_SKU_LEN+1];
                cout << endl << "Please enter the SKU: ";
                cin.getline(inSKU, MAX_SKU_LEN, '\n');
                cout << endl;
                addQty(inSKU);
                cout << endl;

                for(int i=0; i<MAX_NO_RECS; i++)
                {
                    product_[i] = nullptr;
                }
                loadRecs();
            }
            else
            {
                cout << "===Invalid Selection, try again.===" << endl;
                pause();
                cout << endl;
                selection = -1;
            }
        }      

        return 0;
    }
}
