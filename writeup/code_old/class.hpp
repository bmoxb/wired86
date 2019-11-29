class ClassName : public Base {
protected: // Note that visibility keywords should be unindented.
    ClassName(int x, int y); // The first declaration in a class should be the
                             // constructor (assuming the class has one).

public: // Next should be all public declarations with methods before member
        // variables.
    void publicMethod();

    Type method(float z) const;
    Type method(float z, bool a) const; // Group similar methods by not
                                        // separating them with an additional
                                        // newline.

    int member, otherMember; // Only define multiple variables using comma
                             // separation if the two values are closely
                             // related (e.g. pair of x and y coordinates).

protected: // Protected declarations after public.
    using MyType = unsigned int; // Using declarations come before methods.

    virtual void update() = 0;

private: // Finally, all private declarations come last.
    int overridable() override; // If a method in a base class is being
                                // overriden then the override keyword must be
                                // used.

    const MyType privateMember;
};