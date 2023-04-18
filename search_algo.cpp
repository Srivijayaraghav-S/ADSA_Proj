#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>
const int M = 4; // order of the B-tree
using namespace std;
struct Element
{
    string name;
    string category;
    string description;
    int price;
    float rating;
};
struct SearchResult
{
    Element e;
    int total_score;
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
void searchHelper(Node *node, const string &query)
{
    vector<SearchResult> results;
    int i = 0;
    string lq = query;
    transform(lq.begin(), lq.end(), lq.begin(), ::tolower);
    while (i < node->elements.size())
    {
        int score = 0;
        string ln = node->elements[i].name;
        transform(ln.begin(), ln.end(), ln.begin(), ::tolower);
        string ld = node->elements[i].description;
        transform(ld.begin(), ld.end(), ld.begin(), ::tolower);
        string lc = node->elements[i].category;
        transform(lc.begin(), lc.end(), lc.begin(), ::tolower);
        if (ln.find(lq) != string::npos)
        {
            struct SearchResult result;
            result.e = node->elements[i];
            result.total_score = 3;
            results.push_back(node->elements[i]);
            score += 3;
        }
        else if (lc.find(lq) != string::npos)
        {
            results.push_back(node->elements[i]);
            score += 2;
        }
        else if (ld.find(lq) != string::npos)
        {
            results.push_back(node->elements[i]);
            score++;
        }
        i++;
    }
    if (!node->children.empty())
    {
        if (i == node->elements.size() || query <= node->elements[i].name)
        {
            searchHelper(node->children[i], query);
        }
        if (i == 0 || query > node->elements[i - 1].name)
        {
            searchHelper(node->children[i + 1], query);
        }
    }
    priority_queue<SearchResult, vector<SearchResult>, cmp> pq;
    for (auto &result : results)
    {
        pq.push({result, 0});
        if (pq.size() > 10)
        {
            pq.pop();
        }
    }
    std::vector<SearchResult> topResults;
    while (!pq.empty())
    {
        topResults.push_back(pq.top());
        pq.pop();
    }
    reverse(topResults.begin(), topResults.end()); // Higher score is at the end
    return topResults;
}
void search(Node *node, const string &query, vector<Element> &results)
{
    istringstream iss(query);
    string word;
    while (iss >> word)
    {
        searchHelper(node, word);
    }
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
int main(void)
{
    Node *root = nullptr;
    // insert some elements into the B-tree
    Element a = {"Pioneer AVH-2300NEX", "Car and Vehicle Electronics", "This receiver features a 7-inch touchscreen display, Bluetooth connectivity, and compatibility with both Android Auto and Apple CarPlay.", 350, 4.5};
    Element b = {"Escort MAX360C", "Car and Vehicle Electronics", "This radar detector offers 360-degree detection and includes GPS technology for added accuracy. It also features a multi-colour OLED display and comes with a free app for added functionality.", 599, 4.6};
    Element c = {"Garmin DriveSmart 55", "Car and Vehicle Electronics", "This GPS device features a 5.5-inch display, voice-activated navigation, and compatibility with Amazon Alexa. It also includes live traffic updates and alerts for upcoming speed changes.", 199, 4.4};
    Element d = {"JL Audio JX500/1D", "Car and Vehicle Electronics", "This monoblock amplifier is designed to deliver high-quality sound to your car's subwoofer. It features a variable low-pass filter and adjustable bass boost for optimal customization.", 249, 4.8};
    insert(root, a);
    insert(root, b);
    insert(root, c);
    insert(root, d);
    // search for an element in the
    // B-tree by name
    vector<Element> result;
    search(root, "receiver GPS", result);
    cout << result.size() << endl;
    for (auto &element : result)
    {
        cout << element.name << endl;
        cout << element.category << endl;
        cout << element.description << endl;
        cout << element.price << endl;
        cout << element.rating << endl;
    }
    return 0;
}