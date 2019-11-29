// The first enumeration item may appear on the same line as the enum keyword.
// In such an instance, all subsequent items should align with the first.
enum EnumType { FIRST,
                SECOND,
                THIRD };

// Alternatively, all enumeration items may be defined on newlines using
// regular indentation of four spaces.
enum EnumType {
    FIRST,
    SECOND,
    THIRD
};

// If an enumeration only contains a few short items then it is acceptable to
// put it all on a single line.
enum EnumType { FIRST, SECOND, THIRD };