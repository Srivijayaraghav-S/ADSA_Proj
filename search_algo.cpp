#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>
#include <stdlib.h>
#include <limits.h>
const int M = 100; // order of the B-tree
using namespace std;
struct Element
{
    string name;
    string category;
    float price;
    string description;
    float rating;
};
struct ElementComparator
{
    bool operator()(const Element &lhs, const Element &rhs) const
    {
        return lhs.name < rhs.name;
    }
};
struct SearchResult
{
    Element e;
    int total_score;
};
struct sr
{
    int id;
    float price, rating;
};
struct Node
{
    vector<Element> elements;
    vector<Node *> children;
};
// function to search for an element in a B-tree
struct cmp
{
    bool operator()(const SearchResult &lhs, const SearchResult &rhs)
    {
        return lhs.total_score < rhs.total_score;
    }
};
struct node
{
    float min_price;
    float max_price;
    vector<sr> elements;
    node *left;
    node *right;
};
struct node2
{
    float min_rating;
    float max_rating;
    vector<sr> elements2;
    node2 *left;
    node2 *right;
};
vector<SearchResult> searchHelper(Node *node, const string &query)
{
    vector<SearchResult> results;
    int i = 0;
    string lq = query;
    transform(lq.begin(), lq.end(), lq.begin(), ::tolower);
    vector<string> queryWords;
    stringstream ss(lq);
    string word;
    while (ss >> word)
    {
        queryWords.push_back(word);
    }
    while (i < node->elements.size())
    {
        int score = 0;
        string ln = node->elements[i].name;
        transform(ln.begin(), ln.end(), ln.begin(), ::tolower);
        string ld = node->elements[i].description;
        transform(ld.begin(), ld.end(), ld.begin(), ::tolower);
        string lc = node->elements[i].category;
        transform(lc.begin(), lc.end(), lc.begin(), ::tolower);
        int queryWordIndex = 0;
        for (auto &queryWord : queryWords)
        {
            if (ln.find(lq) != string::npos)
            {
                struct SearchResult result;
                result.e = node->elements[i];
                result.total_score = 3;
                results.push_back(result);
                score += 3;
            }
            else if (lc.find(lq) != string::npos)
            {
                struct SearchResult result;
                result.e = node->elements[i];
                result.total_score = 2;
                results.push_back(result);
                score += 2;
            }
            else if (ld.find(lq) != string::npos)
            {
                struct SearchResult result;
                result.e = node->elements[i];
                result.total_score = 1;
                results.push_back(result);
                score++;
            }
            queryWordIndex++;
        }
        i++;
    }
    if (!node->children.empty())
    {
        if (i == node->elements.size() || query <= node->elements[i].name)
        {
            vector<SearchResult> results;
            results = searchHelper(node->children[i], query);
        }
        if (i == 0 || query > node->elements[i].name)
        {
            vector<SearchResult> results;
            results = searchHelper(node->children[i + 1], query);
        }
    }
    priority_queue<SearchResult, vector<SearchResult>, cmp> pq;
    for (auto &result : results)
    {
        // pq.push({result, 0});
        pq.push(result);
        if (pq.size() > 10)
        {
            pq.pop();
        }
    }
    vector<SearchResult> topResults;
    while (!pq.empty())
    {
        topResults.push_back(pq.top());
        pq.pop();
    }
    // reverse(topResults.begin(), topResults.end()); // Higher score is at the end
    return topResults;
}
vector<SearchResult> search(Node *node, const string &query, vector<Element> &results)
{
    istringstream iss(query);
    string word;
    vector<SearchResult> res;
    vector<SearchResult> r;
    while (iss >> word)
    {
        r = searchHelper(node, word);
        vector<SearchResult> r_new = searchHelper(node, word);
        r.insert(r.end(), r_new.begin(), r_new.end());
    }
    set<Element, ElementComparator> seen;
    for (int i = 0; i < r.size(); i++)
    {
        if (seen.find(r[i].e) == seen.end())
        {
            res.push_back(r[i]);
            seen.insert(r[i].e);
        }
    }
    return res;
}

// function to insert an element into a B-tree
void insert(Node *&node, const Element &element)
{
    if (node == nullptr)
    {
        node = new Node;
        node->elements.push_back(element);
        return;
    }
    int i = 0;
    while (i < node->elements.size() && element.name > node->elements[i].name)
    {
        i++;
    }
    if (i < node->elements.size() && element.name == node->elements[i].name)
    {
        // element already exists in the tree
        return;
    }
    if (node->children.empty())
    {
        // leaf node
        node->elements.insert(node->elements.begin() + i, element);
        if (node->elements.size() > M)
        {
            // split the node
            Node *new_node = new Node;
            new_node->elements = vector<Element>(node->elements.begin() + M / 2, node->elements.end());
            node->elements.resize(M / 2);
            if (node == nullptr)
            {
                node = new Node;
                node->elements.push_back(new_node->elements[0]);
                node->children.push_back(new_node);
            }
            else
            {
                node->children.insert(node->children.begin() + i + 1, new_node);
            }
        }
    }
    else
    {
        // non-leaf node
        insert(node->children[i], element);
        if (node->children[i]->elements.size() > M)
        {
            // split the child node
            Node *new_node = new Node;
            new_node->elements = vector<Element>(node->children[i]->elements.begin() + M / 2, node->children[i]->elements.end());
            node->children[i]->elements.resize(M / 2);
            node->children.insert(node->children.begin() + i + 1, new_node);
            node->elements.insert(node->elements.begin() + i, node->children[i]->elements.back());
            node->children[i]->elements.pop_back();
            if (node->elements.size() > M)
            {
                // split the node
                Node *new_root = new Node;
                new_root->elements = vector<Element>(node->elements.begin() + M / 2, node->elements.end());
                new_root->children = vector<Node *>(node->children.begin() + M / 2 + 1, node->children.end());
                node->elements.resize(M / 2);
                node = new_root;
            }
        }
    }
}
std::string SearchResultToJson(const std::vector<SearchResult> &results)
{
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < results.size(); ++i)
    {
        const Element &element = results[i].e;
        ss << "  {\n"
           << "    \"element\": {\n"
           << "      \"name\": \"" << element.name << "\",\n"
           << "      \"category\": \"" << element.category << "\",\n"
           << "      \"price\": " << element.price << ",\n"
           << "      \"description\": \"" << element.description << "\",\n"
           << "      \"rating\": " << element.rating << "\n"
           << "    },\n"
           << "    \"total_score\": " << results[i].total_score << "\n"
           << "  }";
        if (i != results.size() - 1)
        {
            ss << ",";
        }
        ss << "\n";
    }
    ss << "]";
    return ss.str();
}
node *build(vector<sr> &elements, int start, int end)
{
    if (start == end)
    {
        node *leaf = new node{elements[start].price, elements[start].price, {elements[start]}, nullptr, nullptr};
        return leaf;
    }
    int mid = start + (end - start) / 2;
    node *left_child = build(elements, start, mid);
    node *right_child = build(elements, mid + 1, end);
    node *parent = new node{min(left_child->min_price, right_child->min_price),
                            max(left_child->max_price, right_child->max_price),
                            {},
                            left_child,
                            right_child};
    parent->elements.reserve(left_child->elements.size() + right_child->elements.size());
    parent->elements.insert(parent->elements.end(), left_child->elements.begin(), left_child->elements.end());
    parent->elements.insert(parent->elements.end(), right_child->elements.begin(), right_child->elements.end());
    return parent;
}

vector<sr> sr_query_2(node2 *node2, float min_rating, float max_rating)
{
    if (node2 == nullptr)
    {
        return {};
    }
    if (node2->min_rating > max_rating || node2->max_rating < min_rating)
    {
        return {};
    }
    if (node2->min_rating >= min_rating && node2->max_rating <= max_rating)
    {
        return node2->elements2;
    }
    vector<sr> left_results = sr_query_2(node2->left, min_rating, max_rating);
    vector<sr> right_results = sr_query_2(node2->right, min_rating, max_rating);
    vector<sr> results;
    results.reserve(left_results.size() + right_results.size());
    results.insert(results.end(), left_results.begin(), left_results.end());
    results.insert(results.end(), right_results.begin(), right_results.end());
    return results;
}
node2 *build_2(vector<sr> &elements2, int start, int end)
{
    if (start == end)
    {
        node2 *leaf = new node2{elements2[start].rating, elements2[start].rating, {elements2[start]}, nullptr, nullptr};
        return leaf;
    }
    int mid = start + (end - start) / 2;
    node2 *left_child = build_2(elements2, start, mid);
    node2 *right_child = build_2(elements2, mid + 1, end);
    node2 *parent = new node2{min(left_child->min_rating, right_child->min_rating),
                              max(left_child->max_rating, right_child->max_rating),
                              {},
                              left_child,
                              right_child};
    parent->elements2.reserve(left_child->elements2.size() + right_child->elements2.size());
    parent->elements2.insert(parent->elements2.end(), left_child->elements2.begin(), left_child->elements2.end());
    parent->elements2.insert(parent->elements2.end(), right_child->elements2.begin(), right_child->elements2.end());
    return parent;
}
vector<sr> sr_query(node *node, float min_price, float max_price)
{
    if (node == nullptr)
    {
        return {};
    }
    if (node->min_price > max_price || node->max_price < min_price)
    {
        return {};
    }
    if (node->min_price >= min_price && node->max_price <= max_price)
    {
        return node->elements;
    }
    vector<sr> left_results = sr_query(node->left, min_price, max_price);
    vector<sr> right_results = sr_query(node->right, min_price, max_price);
    vector<sr> results;
    results.reserve(left_results.size() + right_results.size());
    results.insert(results.end(), left_results.begin(), left_results.end());
    results.insert(results.end(), right_results.begin(), right_results.end());
    return results;
}
int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }
    Node *root = nullptr;
    // insert some elements into the B-tree
    vector<Element> products;
    products.push_back({"iPhone 12", "Mobile Phones", 999.99, "The latest and greatest iPhone", 4.5});
    products.push_back({"Samsung S21", "Mobile Phones", 799.99, "Top-of-the-line Android phone", 4.3});
    products.push_back({"Google Pixel 5", "Mobile Phones", 699.99, "The best camera on a phone", 4.7});
    products.push_back({"OnePlus 9 Pro", "Mobile Phones", 899.99, "Fast and sleek with a great camera", 4.4});
    products.push_back({"Xiaomi Mi 11", "Mobile Phones", 749.99, "Flagship-level phone at an affordable price", 4.2});
    products.push_back({"Apple iPhone SE", "Mobile Phones", 399.99, "A smaller and more affordable iPhone", 4.1});
    products.push_back({"Samsung Galaxy A52", "Mobile Phones", 449.99, "A great mid-range option from Samsung", 4.0});
    products.push_back({"Nokia Lumia 512", "Mobile Phones", 349.99, "The best camera for the price", 4.6});
    products.push_back({"OnePlus Nord", "Mobile Phones", 549.99, "A budget-friendly phone with premium features", 4.3});
    products.push_back({"Xiaomi Redmi Note 10", "Mobile Phones", 399.99, "An excellent phone for the price", 4.2});

    // Add 20 cameras
    products.push_back({"Sony Alpha A7 III", "Cameras", 1999.99, "A professional-grade full-frame mirrorless camera", 4.9});
    products.push_back({"Canon EOS R5", "Cameras", 3799.99, "The ultimate Canon mirrorless camera", 4.7});
    products.push_back({"Nikon Z6 II", "Cameras", 1999.99, "A versatile and reliable mirrorless camera", 4.6});
    products.push_back({"Fujifilm X-T4", "Cameras", 1799.99, "A compact and powerful mirrorless camera", 4.8});
    products.push_back({"Sony RX100 VII", "Cameras", 1199.99, "A high-end compact camera with a great zoom", 4.4});
    products.push_back({"Canon EOS Rebel T8i", "Cameras", 899.99, "A great entry-level DSLR camera", 4.2});
    products.push_back({"Nikon D780", "Cameras", 2299.99, "A full-frame DSLR camera with fast autofocus", 4.5});
    products.push_back({"Panasonic Lumix GH5", "Cameras", 1699.99, "A great video-focused mirrorless camera", 4.6});
    products.push_back({"Olympus OM-D E-M5 Mark III", "Cameras", 1199.99, "A rugged and portable mirrorless camera", 4.3});
    products.push_back({"Sony ZV-1", "Cameras", 799.99, "A great vlogging camera with excellent autofocus", 4.1});

    // Add 20 smartwatches
    products.push_back({"Apple Series 6", "Smartwatches", 399.99, "The latest and greatest smartwatch from Apple", 4.7});
    products.push_back({"Samsung Galaxy Watch", "Smartwatches", 329.99, "A premium smartwatch with great fitness features", 4.5});
    products.push_back({"Fitbit Versa 3", "Smartwatches", 229.99, "A great fitness-focused smartwatch", 4.3});
    products.push_back({"Garmin Venu 2", "Smartwatches", 399.99, "A great GPS-enabled smartwatch for outdoor activities", 4.6});
    products.push_back({"Fossil Gen 5", "Smartwatches", 249.99, "A stylish and functional Wear OS smartwatch", 4.2});
    products.push_back({"TicWatch 3", "Smartwatches", 299.99, "A great value for a Wear OS smartwatch", 4.0});
    products.push_back({"Huawei Watch GT 2 Pro", "Smartwatches", 299.99, "A premium-looking smartwatch with long battery life", 4.4});
    products.push_back({"Amazfit GTR 2", "Smartwatches", 179.99, "A great budget-friendly option with great battery life", 4.1});
    products.push_back({"Polar Vantage V2", "Smartwatches", 499.99, "A great multisport watch with advanced fitness tracking", 4.5});
    products.push_back({"Suunto 7", "Smartwatches", 399.99, "A great smartwatch for outdoor enthusiasts", 4.3});

    // Add 20 electronic tablets
    products.push_back({"iPad Pro", "Electronic Tablets", 799.99, "The most powerful and versatile iPad", 4.8});
    products.push_back({"Samsung Galaxy Tab S7+", "Electronic Tablets", 849.99, "A great Android tablet with an excellent display", 4.6});
    products.push_back({"Microsoft Surface Pro 7", "Electronic Tablets", 899.99, "A powerful and versatile Windows tablet", 4.5});
    products.push_back({"Amazon Fire HD 10", "Electronic Tablets", 149.99, "A great budget-friendly option for media consumption", 4.2});
    products.push_back({"Lenovo Yoga Smart Tab", "Electronic Tablets", 299.99, "A great Android tablet with a built-in stand", 4.1});
    products.push_back({"Apple iPad Air", "Electronic Tablets", 599.99, "A great balance of power and price for an iPad", 4.4});
    products.push_back({"Huawei MatePad Pro", "Electronic Tablets", 549.99, "A great value for a high-end Android tablet", 4.3});
    products.push_back({"Samsung Galaxy Tab A7", "Electronic Tablets", 229.99, "A great budget-friendly Android tablet", 4.0});
    products.push_back({"Lenovo Tab P11 Pro", "Electronic Tablets", 499.99, "A great mid-range Android tablet", 4.2});
    products.push_back({"Google Pixel Slate", "Electronic Tablets", 899.99, "A great high-end Chrome OS tablet", 4.5});

    // Add 20 laptops
    products.push_back({"Apple MacBook Pro", "Laptops", 1299.99, "The most powerful and versatile MacBook", 4.8});
    products.push_back({"Dell XPS 13", "Laptops", 1099.99, "A great Windows ultrabook with a sleek design", 4.6});
    products.push_back({"HP Spectre x360", "Laptops", 1199.99, "A versatile 2-in-1 laptop with a great display", 4.5});
    products.push_back({"Asus ROG Zephyrus G14", "Laptops", 1199.99, "A great gaming laptop with a powerful AMD processor", 4.2});
    products.push_back({"Lenovo ThinkPad X1 Carbon", "Laptops", 1299.99, "A great business laptop with a durable design", 4.1});
    products.push_back({"Acer Swift 5", "Laptops", 999.99, "A great value for a lightweight Windows laptop", 4.4});
    products.push_back({"Microsoft Surface Laptop 4", "Laptops", 1299.99, "A sleek and powerful Windows laptop", 4.3});
    products.push_back({"Alienware m15 R4", "Laptops", 1999.99, "A high-end gaming laptop with great performance", 4.0});
    products.push_back({"Razer Blade 15", "Laptops", 1499.99, "A great gaming laptop with a slim design", 4.2});
    products.push_back({"LG Gram 17", "Laptops", 1499.99, "A great lightweight laptop with a large display", 4.5});

    // Add 20 headphones
    products.push_back({"Sony WH-1000XM4", "Headphones", 349.99, "The best noise-cancelling headphones on the market", 4.8});
    products.push_back({"Bose QuietComfort 35 II", "Headphones", 299.99, "A great alternative to the Sony WH-1000XM4", 4.6});
    products.push_back({"Apple AirPods Pro", "Headphones", 249.99, "Great true wireless earbuds with noise cancelling", 4.5});
    products.push_back({"Jabra Elite 85t", "Headphones", 229.99, "Great wireless earbuds with great sound quality", 4.2});
    products.push_back({"Sennheiser Momentum 3 Wireless", "Headphones", 399.99, "Great wireless headphones with premium sound quality", 4.1});
    products.push_back({"Bose QuietComfort Earbuds", "Headphones", 279.99, "Great noise-cancelling earbuds with a comfortable fit", 4.4});
    products.push_back({"Beats Solo Pro", "Headphones", 299.99, "Great on-ear headphones with noise cancelling", 4.3});
    products.push_back({"Plantronics BackBeat Go 810", "Headphones", 149.99, "Great over-ear headphones with a long battery life", 4.0});
    products.push_back({"Skullcandy Crusher Evo", "Headphones", 179.99, "Great headphones for bass lovers", 4.2});
    products.push_back({"JBL Free X", "Headphones", 99.99, "Great true wireless earbuds for a budget-friendly price", 4.5});

    // Add 20 speakers
    products.push_back({"Sonos One", "Speakers", 199.99, "Great smart speaker with excellent sound quality", 4.7});
    products.push_back({"Bose SoundLink Revolve", "Speakers", 199.99, "Great portable Bluetooth speaker with 360-degree sound", 4.5});
    products.push_back({"JBL Flip 5", "Speakers", 119.99, "Great waterproof Bluetooth speaker for outdoor use", 4.2});
    products.push_back({"Ultimate Ears Wonderboom 2", "Speakers", 99.99, "Great portable Bluetooth speaker with a fun design", 4.1});
    products.push_back({"Sony SRS-XB23", "Speakers", 99.99, "Great Bluetooth speaker with extra bass", 4.4});
    products.push_back({"Amazon Echo Dot", "Speakers", 49.99, "Great smart speaker with Alexa voice assistant", 4.3});
    products.push_back({"Google Nest Audio", "Speakers", 99.99, "Great smart speaker with Google Assistant", 4.6});
    products.push_back({"Bose Home Speaker 300", "Speakers", 199.99, "Great smart speaker with built-in voice assistants", 4.5});
    products.push_back({"Harman Kardon Onyx Studio 6", "Speakers", 249.99, "Great Bluetooth speaker with a sleek design", 4.0});
    products.push_back({"Marshall Kilburn II", "Speakers", 299.99, "Great portable Bluetooth speaker with a classic design", 4.2});

    // Add 20 TVs
    products.push_back({"LG CX Series 4K OLED TV", "TVs", 1799.99, "Great TV with a stunning OLED display", 4.8});
    products.push_back({"Samsung QN90A QLED TV", "TVs", 2499.99, "Great TV with a bright QLED display", 4.6});
    products.push_back({"Sony X950H 4K TV", "TVs", 1399.99, "Great TV with a bright and colorful display", 4.5});
    products.push_back({"TCL 6-Series 4K TV", "TVs", 899.99, "Great value for a high-quality 4K TV", 4.2});
    products.push_back({"Vizio OLED TV", "TVs", 1299.99, "Great TV with a stunning OLED display at a great price", 4.1});
    products.push_back({"Hisense H9G Quantum Series TV", "TVs", 899.99, "Great TV with a bright and colorful display at a budget-friendly price", 4.4});
    products.push_back({"LG C1 Series 4K OLED TV", "TVs", 1999.99, "Great TV with a stunning OLED display and HDMI 2.1 for gaming", 4.3});
    products.push_back({"Samsung The Frame 4K TV", "TVs", 1499.99, "Great TV with a unique design that looks like a piece of art", 4.0});
    products.push_back({"Sony A8H 4K OLED TV", "TVs", 1999.99, "Great TV with a stunning OLED display and great sound quality", 4.2});
    products.push_back({"Vizio M-Series Quantum 4K TV", "TVs", 699.99, "Great value for a high-quality 4K TV with HDR", 4.5});

    // Add 20 pen drives
    products.push_back({"SanDisk Cruzer Glide USB Flash Drive", "Pen Drives", 9.99, "Great value for a reliable USB flash drive with plenty of storage", 4.4});
    products.push_back({"Samsung BAR Plus USB Flash Drive", "Pen Drives", 14.99, "Great USB flash drive with a durable metal casing", 4.6});
    products.push_back({"Kingston DataTraveler G4 USB Flash Drive", "Pen Drives", 12.99, "Great value for a reliable USB flash drive with a colorful design", 4.3});
    products.push_back({"PNY Turbo Attaché 3 USB Flash Drive", "Pen Drives", 9.99, "Great value for a reliable USB flash drive with fast transfer speeds", 4.2});
    products.push_back({"Lexar JumpDrive S75 USB 3.1 Flash Drive", "Pen Drives", 19.99, "Great USB flash drive with a sleek design and fast transfer speeds", 4.1});
    products.push_back({"SanDisk Ultra Flair USB 3.0 Flash Drive", "Pen Drives", 19.99, "Great value for a reliable USB flash drive with fast transfer speeds and a durable metal casing", 4.4});
    products.push_back({"Samsung FIT Plus USB 3.1 Flash Drive", "Pen Drives", 17.99, "Great USB flash drive with a compact design and fast transfer speeds", 4.5});
    products.push_back({"Kingston DataTraveler 100 G3 USB Flash Drive", "Pen Drives", 14.99, "Great value for a reliable USB flash drive with a large storage capacity", 4.0});
    products.push_back({"PNY Elite-X Fit USB 3.0 Flash Drive", "Pen Drives", 11.99, "Great USB flash drive with a compact design and fast transfer speeds", 4.2});
    products.push_back({"Lexar JumpDrive Tough USB 3.1 Flash Drive", "Pen Drives", 29.99, "Great USB flash drive with a rugged design and fast transfer speeds", 4.1});
    for (int i = 0; i < products.size(); i++)
    {
        insert(root, products[i]);
    }
    // search for an element in the
    // B-tree by name
    vector<Element> result;
    vector<SearchResult> r;
    r = search(root, argv[1], result);
    // vector<pair<Element, float>> p;
    // for (int i = 0; i < r.size(); i++)
    // {
    //     p.push_back(make_pair(r[i].e, r[i].total_score));
    // }
    vector<sr> sr_v;
    for (int i = 0; i < r.size(); i++)
    {
        sr_v.push_back({i, r[i].e.price, r[i].e.rating});
    }
    node *rt = build(sr_v, 0, sr_v.size() - 1);
    float min_price = atof(argv[2]);
    float max_price = atof(argv[3]);
    // float min_price = 14;
    // float max_price = 18;
    vector<sr> results = sr_query(rt, min_price, max_price);
    // for (const sr &e : results)
    // {
    //     cout << e.id << " " << e.price << " " << e.rating << endl;
    // }
    vector<SearchResult> fsr;
    for (const sr &e : results)
    {
        fsr.push_back(r[e.id]);
    }
    vector<sr> sr_v_f;
    for (int i = 0; i < fsr.size(); i++)
    {
        sr_v_f.push_back({i, fsr[i].e.price, fsr[i].e.rating});
    }
    node2 *rt2 = build_2(sr_v_f, 0, sr_v_f.size() - 1);
    float min_rating = atof(argv[4]);
    float max_rating = atof(argv[5]);
    // float min_price = 14;
    // float max_price = 18;
    vector<sr> results2 = sr_query_2(rt2, min_rating, max_rating);
    // for (const sr &e : results)
    // {
    //     cout << e.id << " " << e.price << " " << e.rating << endl;
    // }
    vector<SearchResult> fsr_f;
    for (const sr &e : results2)
    {
        fsr_f.push_back(fsr[e.id]);
    }
    // SegmentTree tree(sr_v.size());
    // for (int i = 0; i < sr_v.size(); i++)
    // {
    //     tree.insert(1, 1, sr_v.size(), sr_v[i]);
    // }
    // vector<sr> filtered_res = tree.query(1, 0, sr_v.size(), 14, 18, 4.3, 4.6);
    // for (int i = 0; i < filtered_res.size(); i++)
    // {
    //     cout << r[filtered_res[i].id].e.name << endl;
    //     cout << r[filtered_res[i].id].e.category << endl;
    //     cout << r[filtered_res[i].id].e.price << endl;
    //     cout << r[filtered_res[i].id].e.description << endl;
    //     cout << r[filtered_res[i].id].e.rating << endl;
    // }
    // for (int i = 0; i < p.size(); i++)
    // {
    //     cout << p[i].first.name << endl;
    //     cout << p[i].first.category << endl;
    //     cout << p[i].first.price << endl;
    //     cout << p[i].first.description << endl;
    //     cout << p[i].first.rating << endl;
    //     cout << p[i].second << endl;
    // }
    // SegmentTree st(sizeof(r));
    // for (int i = 0; i < r.size(); i++)
    // {
    //     st.insert(1, 1, r.size(), r[i]);
    // }

    // cout << r.size() << endl;
    // auto res = st.query(1, 1, r.size(), 14, 18, 4.3, 4.6);
    // cout << res.size() << endl;
    // for (auto s : res)
    // {
    //     cout << s.e.name << endl;
    //     cout << s.e.category << endl;
    //     cout << s.e.price << endl;
    //     cout << s.e.description << endl;
    //     cout << s.e.rating << endl;
    //     cout << s.total_score << endl;
    // }
    // vector<pair<Element, float>> results = tree.search(15.00, 18.00, 4.1, 4.3);
    // for (const auto &p : results)
    // {
    //     cout << "Name: " << p.first.name << endl;
    //     cout << "Category: " << p.first.category << endl;
    //     cout << "Price: " << p.first.price << endl;
    //     cout << "Description: " << p.first.description << endl;
    //     cout << "Rating: " << p.first.rating << endl;
    //     cout << "Total Score: " << p.second << endl;
    //     cout << endl;
    // }
    string finalResult = SearchResultToJson(fsr);
    cout << finalResult << endl;
    return 0;
}