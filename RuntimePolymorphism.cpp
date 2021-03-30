// RuntimePolymorphism.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>

// https://sean-parent.stlab.cc/papers-and-presentations/#better-code-runtime-polymorphism

using namespace std;

template <typename T>
void draw(const T& x, ostream& out, size_t position)
{
    out << string(position, ' ') << x << endl;
}

class object_t {
public:
    template <typename T>
    object_t(T x) : self_(make_shared<model<T>>(move(x)))
    { }

    friend void draw(const object_t& x, ostream& out, size_t position)
    {
        x.self_->draw_(out, position);
    }

    /*void draw2(ostream& out, size_t position) const
    {
        self_->draw_(out, position);
    }*/

private:
    struct concept_t {
        virtual ~concept_t() = default;
        virtual void draw_(ostream&, size_t) const = 0;
    };
    template <typename T>
    struct model final : concept_t {
        model(T x) : data_(move(x)) { }
        void draw_(ostream& out, size_t position) const override
        {
            draw(data_, out, position);
        }

        T data_;
    };

    shared_ptr<const concept_t> self_;
};

using document_t = vector<object_t>;

void draw(const document_t& x, ostream& out, size_t position)
{
    out << string(position, ' ') << "<document>" << endl;
    for (const auto& e : x)
    {
        draw(e, out, position + 2);
        //e.draw2(out, position + 2);
    }
    out << string(position, ' ') << "</document>" << endl;
}

class my_class_t {
    /* ... */
};

void draw(const my_class_t&, ostream& out, size_t position)
{
    out << string(position, ' ') << "my_class_t" << endl;
}

int main()
{
    document_t document;

    document.emplace_back(0);
    document.emplace_back(string("Hello!"));
    document.emplace_back(document);
    document.emplace_back(my_class_t());

    draw(document, cout, 0);
}
