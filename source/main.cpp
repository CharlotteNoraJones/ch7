// #include <iostream>

// // Ch.7 Notes

// //7.1 Intro
// // templates: class or function parameterized with a set of types or values

// // 7.2 Parameterized Types
// // example of template vector

// #include <stdexcept>
// #include <string>
// #include <list>

// template<typename T> // makes T a type parameter of the declaration it prefixes. 
// class Vector {
//     private:
//         T* elem; // elem points to an array of sz elements of type T
//         int sz;
//     public:
//         explicit Vector(int s); // constructor: establish invariant, acquire resources
//         ~Vector() { delete[] elem; } // destructor, release resources

//         // ... copy and move operations ...

//         T& operator[](int i); // non-const Vectors
//         const T& operator[](int i) const; // for const Vectors
//         int size() const {return sz;}
// };


// // template<class T> is an older version of the modern template<typename T>

// // defining member functions
// template<typename T>
// Vector<T>::Vector(int s)
// {
//     if (s<0)
//         throw std::length_error{"Vector constructor: negative size"};
//     elem = new T[s];
//     sz = s;
// }

// template<typename T>
// const T& Vector<T>::operator[](int i) const // const at the end makes it a compiler error for the function to change a data member. 
// {
//     if (i<0 || size()<=i)
//         throw std::out_of_range{"Vector::operator[]"};
//     return elem[i];
// }

// // adding begin() and end() to support range-for loop
// template<typename T>
// T* begin(Vector<T>& x)
// {
//     return &x[0]; // pointer to first element or to one-past-the-last element
// }

// template<typename T>
// T* end(Vector<T>& x)
// {
//     return &x[0]+x.size(); // pointer to one-past-the-last element
// }

// // with the definition above, Vectors can be defined like so

// Vector<char> vc(200); // vector of 200 characters
// Vector<std::string> vs(17); // vector of 17 strings
// Vector<std::list<int>> vli(45); // vector of 45 lists of integers. 

// // Vectors can be used like so

// void write(const Vector<std::string>& vs) // Vector of some strings
// {
//     for(int i = 0; i!=vs.size(); ++i)
//         std::cout << vs[i] << "\n";
// }

// // begin() and end() member functions let us use the range for function

// void write2(Vector<std::string>& vs) // Vector of some strings
// {
//     for (auto& s : vs)
//         std::cout << s << "\n";
// }

// // templates are compile time. 
// // instantiation/specialization = template + template arguements
// // instantiation time: when the compiler generates code for instantiation

// // 7.2.1 Constrained Template Arguments
// // Element keyword specifies requirements for template objects

// template<Element T>
// class Vector {
//     private:
//         T* elem; // elem points to an array of sz elements of type T
//         int sz;
//         // ..
// };

// // Element is a predicate that checks whether T has all the properties the a Vector requires. 
// // this is called a concept. 
// // constrained arguement: template arguement that the concept is sqecified on
// // constrained template: template with a concept. 

// // Element could be std::copyable, for example

// // with Element as std::copyable
// Vector<int> v1; // Ok: we can copy an int
// Vector<thread> v2; // Error: thread's can't be copied

// // concept checking is C++ 20 feature

// 7.2.2 Value Template Arguments

// templates can take value arguements too
// template<typename T, int N>
// struct Buffer {
//     constexpr int size() {return N;}
//     T elem[N];
//     // ..
// };

// // a use case is Buffer
// // Buffer allows arbitrarily sized buffers with no use of the free store (dynamic memory)

// Buffer<char,1024> glob; // global buffer of characters (statically allocated)

// void fct()
// {
//     Buffer<int,10> buf; // local buffer of integers (on the stack)
//     // ..
// }

// // a string literal can't be a template value arguement
// // if you need to, use an array of chars

// template<char* s>
// void outs() {std::cout << s;}

// char arr[] = "Weird workaround!";

// void use()
// {
//     outs<"straightwordard use">(); // error(for now)
//     outs<arr>(); // writes: Weird workaround!
// }

// // 7.2.3 Template Argument Deduction
// // when defining a type as an instantiation of a template, must specify its template arguements

// #include <utility>
// #include <vector>
// #include <string>

// std::pair<int, double> p {1, 5.2};

// // often, we can let the constructor deduce the template arguements' types

// std::pair p2 {1, 5.2};

// // example with a container

// template<typename T>
// class Vector {
//     public:
//         Vector(int);
//         Vector(initializer_list<T>); // initializer-list constructor
//         // ..
// };

// std::vector v1 {1, 2, 3}; // deduce v1's element type from the initializer element type: int
// std::vector v2 {v1}; // deduce v2's element type from v1's element type: int

// auto p {new std::vector{1, 2, 3}}; // p is a Vector<int>

// std::vector<int> v3(1); // here we need to be explicit about the element type (no element type is mentioned)

// // deduction is not always perfect
// std::vector<std::string> vs {"Hello", "World"}; // Ok: std::vector<std::string>
// std::vector vs1 {"Hello", "World"}; // Ok: deduces to Vector<const char*> (Surpise?)
// std::vector vs2 {"Hello"s, "World"s}; // Ok: deduces to std::vector<std::string>
// std::vector vs3 {"Hello"s, "World"}; // error: the initializer list is not homogenous
// std::vector<std::string> vs4 {"Hello"s, "World"}; // Ok: the element type is explicit

// // C-style string literal is const char*

// // resolving an ambiguity
// template<typename T>
// class Vector {
//     public: 
//         std::vector(initializer_list<T>); // initializer-list constructor

//         template<typename Iter>
//             std::vector(Iter b, Iter e); // (b:e) iterator-pair constructor
        
//         struct iterator { using value_type = T /* ... */};
//         iterator begin();
//         // ..
// };

// Vector v1 {1, 2, 3, 4, 5}; // element type is int
// Vector v2 {v1.begin(), v1.begin()+2} // a pair of iterators or a pair of values (of type iterator)?
// Vector v3(9, 17); // error: ambiguous

// // adding a deduction guide to tell the compiler that a pair of values of the same type should be considered iterators

// template<typename Iter>
// Vector(Iter,Iter) -> Vector<typename Iter::value_type>;

// Vector v1 {1, 2, 3, 4, 5}; // element type is int
// Vector v2 {v1.begin(), v1.begin()+2}; // pair-of-iterators: element type is int
// Vector v3 {v1.begin(), v1.begin()+2}; // element type is Vector2::iterator

// {} always prefers the initializer_list constructor if it is present. 
// v3 is thus a vector of iterators Vector<Vector<int>::iterator>
// () is used if we don't want an initializer_list

// it's best to design class templates so that deduction guides arn't needed

//CTAD = class template arguement deducation

// 7.3 Parameterized Operations
// templates are used to parameterize types and templates in the standard library
// ways of expressing an operation parameterized by types or values
// function template
// function object: an object that an carry data and be called like a function
// lambda expression: shorthand notation for a function object

// 7.3.1 Function Templates
template<typename Sequence, typename Value>
Value sum(const Sequence& s, Value v)
{
    for (auto x: s)
        v+=x;
    return v;
}

// value template arguement and the function arguement v allow the caller to specify the 
// tpye and value of accumulator

#include <vector>
#include <list>
#include <complex>
#include <iostream>
#include <algorithm>

void user(std::vector<int>& vi, std::list<double>& Id, std::vector<std::complex<double>>& vc){
    int x = sum(vi, 0); // the sum of a vector of ints (add ints)
    double d = sum(vi, 0.0); // the sum of the vector of ints (add doubles)
    double dd = sum(Id, 0.0); // the sum of a list of doubles
    auto z = sum(vc, std::complex{0.0,0.0}); // the sum of a vector of complex <double>s
}

// adding ints to a double better handles sums larger than the largest int. 

// function template can be a member function, but not a virtual member
// this is because the compliler couldn't generate a vtbl, because it wound'nt know all instantiations. 

// 7.3.2
// function object (functor) defines objects that can be called like functions
template <typename T>
class Less_than {
    const T val; // value to compare against
    public:
        Less_than(const T& v) : val{v} {}
        bool operator()(const T& x) const {return x<val} // call operator
};

// operator() implements application operator, also called "function call" or "call"

// we can now create named variables of type Less_than 
Less_than Iti {42}; // Iti(i) will compare i to 42 using < (i<42)
Less_than Its {"Backus"s}; // Its(s) will compare s to "Backus" using < (s<"Backus")
Less_than<std::string> Its2 {"Naur"}; // "Naur" is a C-styl stirng, so we need <string> to the get the right < 

// a function object can be called just like a function

void fct(int n, const std::string& s)
{
    bool b1 = Iti(n); // true if n<42
    bool b2 = Its(s); // true if s<"Backus"
    // ..
}

// funciton arguements can be used as arguments to algorithms
template<typename C, typename P>
int count<const C& c, P pred) // assume that C is a container and P is a predicate on this elements
{
    int cnt = 0;
    for (const auto& x : c)
        if (pred(x))
            ++cnt;
    return cnt;
}

// above is a simplified version of the std count_if algorithim

// predicate: something that we can invoke to return true or false

// example of a predicate
void f(const std::vector<int>& vec, const std::list<string>& lst, int x, const string& s)
{
    std::cout << "number of values less than " << x << ": " << count(vec, Less_than{x}) << "\n";
    std::cout << "number of values less than " << s << ": " << count(lst, Less_than{s}) << "\n";
}

// Less_than{x} constructs an object of type Less_than<int>

// function objects carry the value that they compare against. 

// advantages
// don't have to introduce global variables 
// don't have to write a seperate function for each value and type
// for a simple function object, inlining is easy for the compiler, making them more efficient than an indirect function call. 
// efficient + carry data makes them good arguements for algorithims

// policy object: function object used to specify the meaning of key operations of a general algorithm

// 7.3.3 Lambda Expressions

// notation to implicitly generate function objects

void f(const std::vector<int>& vec, const std::list<std::string>& lst, int x, const std::string& s)
{
    std::cout << "number of values less than " << x
        << ": " << std::count(vec,[&](int a){ return a<x; })
        << '\n';
    std::cout << "number of values less than" << s
        << ": " << std::count(lst,[&](const string& a){return a<s})
        << "\n";
}

// lambda expression: [&](int a){ return a<x; }
// lambdas generate function objects
// [&]: capture list
// specifies all local names will be accessed through references
// to only capture x, use [&x]
// capture nothing is []
// to give a copy of x, use [x]
// capture all local by value is [=]

// for lambda within a member function [this] captures object by refernce.
// [*this] makes a copy of the current object

// list objects to capture them in a member function

// 7.3.3.1 Lambdas as function arguements
// nontrivial operations should be named, not lambdas

// function objects allow seperating the traversal othe container form the specification of 
// what needs to be done to each element

// function that operates on each object pointed to by a container of pointers

template<typename C, typename Oper>
void for_each(C& c, Oper op) // assume that C is a container of pointers
{
    for (auto& x : c)
        op(x); // pass op() a reference to each element pointed to
}

#include<memory>

// example
void user()
{
    std::vector<std::unique_ptr<Shape>> v; // Shape object from ch.5
    while(cin)
        v.push_back(read_shape(cin));
    for_each(v,[](std::unique_ptr<Shape>& ps) {ps->draw();}); // draw_all()
    for_each(v,[](std::unique_ptr<Shape>& ps){ps->rotate(45);}); // rotate_all(45)
}
    // pass the unique_ptr<Shape> by reference so the for_each doens't have to deal with a lifetime issue

// a lambda can be generic
template<class S>
void rotate_and_draw(std::vector<S>& v, int r)
{
    for_each(v,[](auto& s){s->rotate(r);s->draw();});
}

// auto means that any type can be an initializer
// lambda with auto: generic lambda
// the parameter can be contrained with a concept

// for example
for_each(v,[](Pointer_to_class auto& s){s -> rotate(r); s->draw();}; // Pointer to class is user-defined concept

void user()
{
    std::vector<std::unique_ptr<Shape>> v1;
    std::vector<Shape*> v2;
    // ..
    rotate_and_draw(v1, 45);
    rotate_and_draw(v2, 90);
}

// we could define a Pointer_to_Shape concept to be more specific
// it would also allow us to use classes that weren't defined from class Shape
// as long as they had the correct properties

// 7.3.3.2
// Lambdas for initialization
// lambas let us turn any statement into an expression

// example of a complicated initialization
enum class Init_mode {zero, seq, cpy, patrn}; // initializer alternatives

void user(Init_mode m, int n, std::vector<int>& arg, Iterator p, Iterator q)
{
    std::vector<int> v;

    // messy initialization code:

    switch(m) {
        case zero:
            v=std::vector<int>(n); // n elements initialized to 0
            break;
        case cpy:
            v = arg;
            break;
    };

    // .. 

    if (m == seq)
        v.assign(p, q); // copy from sequence [p:q]

    // ..
}

// this code is complicated and has the following disadvantages
// variable could be used before it gets its intended value
// "initialization code" could get mixed with other code. 
// this isn't initialization, it's assignment

// instead, convert to lambda using an initializer
void user(Init_mode m, int n, std::vector<int>& arg, Iterator p, Iterator q)
{
    std::vector<int> v = [&] {
        switch (m) {
            case zero: return std::vector<int>(n); // n elements initialized to 0
            case seq: return std::vector<int>{p,q}; // copy froms sequence [p:q)
            case cpy: return arg;
            }
    }();
}

// 7.3.3.3 Finally
// destructors offer an easy and useful mechanism for cleaning up after an object (RAII)
// what if we need to do clean up when...
// that data is not associated with just one object
// the object doens't have a destructor (becuase its a type shared with a C program)

// define finally(), that takes an action to be executed on the exit from the scope

void old_style(int n)
{
    void* p = malloc(n*sizeof(int)); // c-style
    auto act = finally([&]{free(p);}); // call the lambda upon scope exit
    // ..
    // p is implicitly freed upon scope exit
};

// this is ad hoc, but butter than trying to call free(p) on all exits from the function. 

// example of the finally function
template <class F>
[[nodiscard]] auto finally(F f)
{
    return Final_action{f};
}

// [[nodiscard]] keeps users from forgetting to copy a generated Final_action into the scope for which the action is intended. 

// the final action class can look like this

template <class F>
struct Final_action {
    explicit Final_action(F f): act(f) {}
    ~Final_action() {act();}
    F act;
};

// Core Guidelines Support Librart (GSL) has a finally()


// 7.4 Template Mechanisms
// language features to define good templates:
// variable templates: values dependant on a type
// alias templates: aliases for types and templates
// if constexpr: compile-time selection mechanism
// requires-expressions: compile time mechanism to inquire about types and expressions

// constexpr functions and static_asserts often take part in template design and use

// 7.4.1 Variable Templates
// when using a type, we often ws constants and values of the type
// this is also the case when using a class template

// example:
template <class T>
    constexpr T viscosity = 0.4;

template <class T>
    constexpr space_vector<T> external_acceleration = { T{}, T{-9.8}, T{} };

auto vis2 = 2*viscosity<double>;
auto acc = external_acceleration<float>;

// space_vector is a 3d vector

// arbitrary expressions of suitable tyles can be used as initializers
template<typename T, typename T2>
constexpr bool Assignable = is_assignable<T&,T2>::value; // is_assignable is a type trait

template<typenameT>
void testing()
{
    static_assert(Assignable<T&,double>, "can't assign a double to a T");
    static_assert(Assignable<T&,string>, "can't assign a string to a T");
}

// std uses variable templates to provide mathematical constants, such as pi and log2e

// 7.4.2 Aliases
// it's often useful to introduce a synonym for a type or template

// example: <sdtddef> contains alias for size_t
using size_t = unsigned int;

// size_t is implementation dependant, so in another implementation size_t could be unsigned long
// tis alias thus allows the programmer to write more portable code

// a parametereized type can provide an alias for types related to their template arguements

template<typename T>
class Vector {
    public:
        using value_type = T;
        // ..
};

// std containers all provide value_type 
// we can thus write code for every container that follows the convention

template<typename C>
using Value_type = C::value_type; // the type of C's elements

template<typename Container>
void algo(Container& c)
{
    std::vector<Value_type<Container>> vec; // keep results here
    // ..
}

// this value_type is a simplified range_value_t

// aliasing can define a new template by binding some or all template arguements
// example: 

template<typename Key, typename Value>
class Map {
    // ..
};

template<typename Value>
using Stirng_map = Map<string,Value>;

String_map<int> m; // m is a Map<string,int?

// 7.4.3 Compile-Time if
// imagine an operation that can be implemented with one of two operations
// slow_and_safe(T) or simple_and_fast(T)

// one option, slow_and_safe is provided by parent class, child class an override with simple_and_fase if necessary

// alternativly, we can use a compile-time if

template<typename T>
void update(T& target)
{
    // ..
    if constexpr(is_trivially_copyable_v<T>)
        simple_and_fast(target); // for "plain old data"
    else  
        slow_and_safe(target); // for more complex types
    // ..
}

// is_trivially_copyable_v<T> is a type predicate 
// tells if a type can be trivially copied. 

// only the selected branch of if constexpr is checked by the compiler
// this offers optimal performance and locality of the optimization

// if constexpr doesn't break any usual rules; it's not a text-manipulation mechanism

// this code is bad and stupid. if constexpr does not work like this

template<typename T>
void bad(T arg)
{
    if constexpr(!is_trivially_copyable_v<T>)
        try { // Opp,s the if extends beyond this line
    
    g(arg);

    if constexpr(!is_trivially_copyable_v<T>)
        } catch(...) { "/* ... */"} // syntax error
}

// permitting text manipulation would create a lot of problems
// it would break abstracy syntax trees, for example

// this is a cleaner solution

template<typename T>
void good(T arg)
{
    if constexpr (is_trivially_copyable_v<T>)
        g(arg);
    else 
        try
        {
            g(arg);
        }
        catch(...)
        {
            /* ... */
        }       
}