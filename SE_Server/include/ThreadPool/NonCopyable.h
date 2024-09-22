#pragma once

//不允许复制或者赋值
/*
    protected 访问级别表示成员可以在派生类中访问，
    但不能在类的外部访问。
    这意味着，只有派生类和基类内部的成员函数可以访问 protected 成员。
*/
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};