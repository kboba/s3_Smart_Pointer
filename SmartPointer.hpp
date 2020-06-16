#pragma once

template<class T>
class SmartPointer
{
    class ReferenceCounter
    {
        int counter_;

    public:
        ReferenceCounter() : counter_(0) {}
        ReferenceCounter(int counter_) : counter_(counter_) {}

        void AddRef() { counter_++; }
        void DelRef() { counter_--; }
        int Count() { return counter_; }
    };

    T* ptr;
    ReferenceCounter* counter;

public:
    SmartPointer() : ptr(nullptr), counter(nullptr) {}
    SmartPointer(T* ptr) : ptr(ptr), counter(new ReferenceCounter(1)) {}
    SmartPointer(const SmartPointer<T>& copy) : ptr(copy.ptr), counter(copy.counter) 
    {
        if (counter == nullptr)
            counter = new ReferenceCounter(1);
        else
            counter->AddRef();
    }

    ~SmartPointer()
    {
        if (counter)
        {
            counter->DelRef();
            if (counter->Count() == 0)
                delete ptr;
        }
    }

    SmartPointer& operator=(const SmartPointer& sp)
    {
        if (ptr != sp.ptr)
        {
            if (ptr == nullptr)
            {
                ptr = sp.ptr;
                counter = sp.counter;
                counter->AddRef();
            }
            else if (ptr != nullptr)
            {
                if (counter->Count()==1)
                {
                    delete ptr;
                    counter->DelRef();
                    delete counter;
                    ptr = sp.ptr;
                    counter = sp.counter;
                    counter->AddRef();
                }
                else
                {
                    ptr = sp.ptr;
                    counter->DelRef();
                    counter = sp.counter;
                    counter->AddRef();
                }
            }
        }
        return *this;
    }
    
    T* operator->()
    {
        return ptr;
    }
    
    T* operator*()
    {
        return ptr;
    }

    bool operator bool()
    {
        return ptr != nullptr;
    }
    
    void reset()
    {
        if (counter->count()==0 || counter==nullptr) { }
        else if (counter->count() == 1)
        {
            delete ptr;
            delete counter;
            ptr = nullptr;
            counter = nullptr;
        }
        else
        {
            counter->DelRef();
            ptr = nullptr;
            counter = nullptr;
        }
    }
    
    void reset(T* ptr_)
    {
        reset();
        ptr = ptr_;
        counter = new ReferenceCounter(1);
    }

    void swap(SmartPointer& r)
    {
        T* help_t = ptr;
        ReferenceCounter* help_rc = counter;
        ptr = r.ptr;
        counter = r.counter;
        r.ptr = help_t;
        r.counter = help_rc;
    }
};

template<class t>
SmartPointer<t> make_smart_pointer(t* ptr)
{
    return SmartPointer<t>(ptr);
}

template <typename T, typename... Args>
SmartPointer<T> make_smart_pointer(Args... args) 
{
    SmartPointer<T> p(new T(args...));
    return p;
}
