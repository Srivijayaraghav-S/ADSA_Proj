#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Product
{
    string name;
    string category;
    float price;
    string description;
    int rating;
};

int main()
{
    // Define the categories and generate the product names randomly
    string categories[9] = {"mobile phones", "cameras", "smartwatches", "electronic tablets", "laptops", "headphones", "speakers", "TVs", "pen drives"};
    string names[200] = {"Galaxy", "Pixel", "iPhone", "Sony", "Nikon", "Canon", "Garmin", "Apple", "Samsung", "Kindle", "Lenovo", "HP", "Bose", "JBL", "LG", "SanDisk", "Kingston", "Toshiba", "Philips", "Huawei", "GoPro", "Fitbit", "iPad", "MacBook", "AirPods", "Xbox", "PlayStation", "Nintendo", "Seagate", "Western Digital", "Roku", "FireStick", "Echo", "Google Home", "Chromecast", "Sonos", "Beats", "Sennheiser", "Bowers & Wilkins", "Dell", "Acer", "Asus", "Microsoft", "Surface", "Logitech", "Razer", "HyperX", "Corsair", "Netgear", "Linksys", "TP-Link", "Belkin", "Anker", "Jabra", "Sony Xperia", "Moto G", "LG G", "Google Pixel", "OnePlus", "Redmi", "Realme", "Oppo", "Vivo", "Poco", "Nokia", "Motorola", "Sony Alpha", "Canon EOS", "Nikon D", "Fujifilm X", "Pentax K", "GoPro Hero", "DJI Osmo", "Garmin Forerunner", "Apple Watch", "Fitbit Versa", "Samsung Galaxy Watch", "Lenovo Yoga", "HP Pavilion", "Dell Inspiron", "Acer Nitro", "Asus ROG", "MSI", "Razer Blade", "Corsair One", "Logitech G", "Xbox Elite", "PlayStation DualSense", "Nintendo Switch Pro", "SanDisk Ultra", "Kingston Canvas", "Toshiba Canvio", "Seagate Backup Plus", "Western Digital My Passport", "Roku Streaming Stick", "Amazon FireStick 4K", "Echo Dot", "Google Nest Mini", "Chromecast Ultra", "Sonos One", "Bose SoundLink", "JBL Flip", "LG XBoom", "Samsung Harman Kardon", "Philips Hue", "Netgear Nighthawk", "Linksys Velop", "TP-Link Deco", "Belkin Boost", "Anker PowerCore", "Sony WH-1000XM4", "Bose QuietComfort", "Sennheiser Momentum", "Jabra Elite", "Bowers & Wilkins PX", "Beats Solo", "HyperX Cloud", "Corsair HS", "Razer Kraken"};

    // Initialize the random number generator
    srand(time(NULL));

    // Create a vector to hold the products
    vector<Product> products;

    // Generate 20 products for each category
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            // Generate a random name for the product
            string name = names[rand() % 200];
            // Make sure the name doesn't contain more than one word in common with any other product in the vector
            bool valid_name;
            do
            {
                valid_name = true;
                for (int k = 0; k < products.size(); k++)
                {
                    int common_words = 0;
                    for (int l = 0; l < name.length(); l++)
                    {
                        if (name[l] == ' ' && name[l + 1] != '\0')
                        {
                            if (products[k].name.find(name.substr(0, l)) != string::npos)
                            {
                                common_words++;
                            }
                        }
                    }
                    if (common_words > 1)
                    {
                        valid_name = false;
                        name = names[rand() % 200];
                        break;
                    }
                }
            } while (!valid_name);

            // Create the product struct and add it to the vector
            Product p;
            p.name = name;
            p.category = categories[i];
            p.price = (rand() % 1000) + 1;
            p.description = "This is a " + name + " " + categories[i] + ".";
            p.rating = rand() % 5 + 1;
            products.push_back(p);
        }
    }

    // Print the products
    for (int i = 0; i < products.size(); i++)
    {
        cout << "Name: " << products[i].name << endl;
        cout << "Category: " << products[i].category << endl;
        cout << "Price: $" << products[i].price << endl;
        cout << "Description: " << products[i].description << endl;
        cout << "Rating: " << products[i].rating << "/5" << endl;
        cout << endl;
    }

    return 0;
}