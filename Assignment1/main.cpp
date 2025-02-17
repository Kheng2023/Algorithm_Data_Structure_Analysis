#include <iostream>
#include <string>

using namespace std;

// School method addition function
string addition(string num1, string num2, int base) 
{
    int carry = 0;
    string result;

    int i = num1.size() - 1;
    int j = num2.size() - 1;

    while (i >= 0 || j >= 0 || carry) 
    {
        /*convert each digit from string to int starting from smallest digit
        if over the length of the num replace with 0 */
        int digit1 = (i >= 0) ? num1[i--] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j--] - '0' : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        sum %= base;

        result = char(sum + '0') + result;
    }

    return result;
}

string subtraction(string num1, string num2, int base) {
    int borrow = 0;
    string result;

    int i = num1.size() - 1; // Index of num1
    int j = num2.size() - 1; // Index of num2

    while (i >= 0 || j >= 0) {
        int digit1 = (i >= 0) ? num1[i--] - '0' : 0;
        int digit2 = (j >= 0) ? num2[j--] - '0' : 0;

        // Apply borrow if necessary
        digit1 -= borrow;
        borrow = 0;

        if (digit1 < digit2) {
            borrow = 1;
            digit1 += base;
        }

        int diff = digit1 - digit2;

        result = char(diff + '0') + result;
    }

    //remove leading 0 if needed
    while (result.size() > 1 && result[0] == '0') {
        result.erase(result.begin());
    }

    return result;
}

// Function to perform multiplication using the Karatsuba algorithm
string karatsuba(string a, string b, int base) 
{
    int n = max(a.size(), b.size());

    int lena=a.size();
    int lenb=b.size();

    //pad leading 0s for shorter string
    if (lena < lenb)
    {
        for (int i = 0 ; i < (lenb-lena) ; i++)
            a = '0' + a;
    }
    else if (lenb < lena)
    {
        for (int i = 0 ; i < (lena-lenb) ; i++)
            b = '0' + b;
    }

    // Base cases
    if (n==0) return 0;
    if (n == 1) 
    {
    int product = (a[0] - '0') * (b[0] - '0');
    int carry = product / base;
    product %= base;

    // Convert the result back to string representation
    string result = string(1, char(carry + '0')) + string(1, char(product + '0'));
    return result;
    }

    int k = n / 2;

    // Split a and b into a1, a0, b1, and b0
    string a1 = a.substr(0, k); // Extract substring starting from index 0 until k-1 
    string a0 = a.substr(k); // Extract substring starting from index k until the end
    string b1 = b.substr(0, k); //n-k of most significant bits
    string b0 = b.substr(k); //k of least significant bits

    // Calculate the three multiplications
    string p2 = karatsuba(a1, b1, base);
    string p0 = karatsuba(a0, b0, base);

    string aSum = addition(a0, a1, base);
    string bSum = addition(b0, b1, base);
    string p1 = karatsuba(aSum, bSum, base);
    p1 = subtraction(p1, addition(p2, p0, base), base);

    p2 += string(2 * (n - k), '0');
    p1 += string(n - k, '0');

    string result = addition(addition(p2, p0, base), p1, base);

    //remove leading 0 if needed
    while (result.size() > 1 && result[0] == '0') {
        result.erase(result.begin());
    }

    return result;
}
int compareStrings(const string a, const string b) {
    //compare length
    if (a.size() < b.size())
        return -1;
    if (a.size() > b.size())
        return 1;
    
    //compare the digit if length is the same
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] < b[i])
            return -1;
        if (a[i] > b[i])
            return 1;
    }
    
    return 0; // Both strings are equal
}

string division(string a, string b, int base) {
    if (int(compareStrings(a, b)) == -1)
        return "0"; //if divisor is bigger than the dividend

    string quotient;
    string remainder;

    for (int i = 0; i < a.size(); ++i) 
    {
        remainder += a[i];
        /*remove leading 0s otherwise it will incorrectly think that the remainder is larger 
        than the divisor, noted during debugging some remainder had leading 0s */
        while (remainder.size() > 1 && remainder[0] == '0') {
        remainder.erase(remainder.begin());
        }

        string count = "0";
        while (int(compareStrings(remainder, b)) >= 0) 
        {
            remainder = subtraction(remainder, b, base);
            count = addition(count, "1", base); 
        }
        quotient += count;
    }

    while (quotient.size() > 1 && quotient[0] == '0') {
        quotient.erase(quotient.begin());
    }

    return quotient;
}

int main() 
{
    string I1, I2;
    int base;
    cin >> I1 >> I2 >> base;

    string sum = addition(I1, I2, base);
    string product = karatsuba(I1, I2, base);
    string div = division(I1, I2, base);

    cout << sum << " " << product << " " << div << endl;

    return 0;
}