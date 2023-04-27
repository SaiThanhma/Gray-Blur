namespace mymath{

    template<typename T = int>
    constexpr T factorial(T value);

    template<typename T = double>
    constexpr T pow(T base, int exponent);

    template<typename T = double>
    constexpr T exp(double value);



    template<typename T>
    constexpr T factorial(T value){
        return value <= 1 ? 1 : value * factorial(value - 1);
    }
    
    template<typename T>
    constexpr T pow(T base, int exponent){
        return exponent <= 0 ? 1 : base * pow(base, exponent - 1);
    }

    template<typename T>
    constexpr T exp(double value){
        double res = 1;
        for(int i = 1; i < 21; ++i){
            res += pow(value, i)/factorial<long>(i);
        }
        return res;
    }
}
