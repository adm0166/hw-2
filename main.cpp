#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

// Turns a number into a dollar string like "$965.00".
// Rounds to the nearest cent first (rounding .5 up) so it matches how
// money is normally rounded, instead of leaving it to floating-point rounding.
string money(double amount)
{
	double rounded = (double)((long long)(amount * 100.0 + 0.5)) / 100.0;
	ostringstream oss;
	oss << fixed << setprecision(2) << "$" << rounded;
	return oss.str();
}

// Turns the rate into a plain string like "1.5" (no forced decimals)
string rateStr(double rate)
{
	ostringstream oss;
	oss << rate;
	return oss.str();
}

int main(int argc, char* argv[])
{
	if (argc > 4)
	{
		cout << "Too many arguments. Cannot pass in more than three." << endl;
		return -1;
	}

	int i = 1;
	double loan_amount, yearly_interest_rate, monthly_payment;
	double arguments[3];

	while (i < argc)
	{
		try
		{
			arguments[i - 1] = stod(argv[i]);
		}
		catch (const std::invalid_argument&)
		{
			if (i == 1)
				cout << "(Invalid loan amount): " << argv[i] << endl;
			else if (i == 2)
				cout << "(Invalid interest rate): " << argv[i - 1] << " " << argv[i] << endl;
			else
				cout << "(Invalid payment): " << argv[i - 2] << " " << argv[i - 1] << " " << argv[i] << endl;
			return -2;
		}

		if (i == 1 && arguments[0] <= 0)
		{
			cout << "(Invalid loan amount): loan amount must be positive." << endl;
			return -3;
		}
		if (i == 2 && arguments[1] < 0)
		{
			cout << "(Invalid interest rate): interest rate cannot be negative." << endl;
			return -3;
		}
		if (i == 3 && arguments[2] <= 0)
		{
			cout << "(Invalid payment): monthly payment must be positive." << endl;
			return -3;
		}

		i++;
	}

	if (argc - 1 < 3)
	{
		cout << "Not enough arguments." << endl;
		return -1;
	}

	loan_amount = arguments[0];
	yearly_interest_rate = arguments[1];
	monthly_payment = arguments[2];

	// yearly rate -> monthly rate. Example: 18% / year = 1.5% / month
	double monthly_rate_percent = yearly_interest_rate / 12;
	double monthly_rate = monthly_rate_percent / 100;

	double first_month_interest = loan_amount * monthly_rate;
	if (monthly_payment <= first_month_interest)
	{
		cout << "(Insufficient payment): the payment does not cover the interest, so this loan would never be paid off." << endl;
		return -4;
	}

	double balance = loan_amount;
	double total_interest = 0;
	int month = 0;

	// Column widths used for every row, so things always line up
	const int W_MONTH = 7;
	const int W_BALANCE = 14;
	const int W_PAYMENT = 12;
	const int W_RATE = 8;
	const int W_INTEREST = 12;
	const int W_PRINCIPAL = 12;

	cout << "*****************************************************************\n";
	cout << "\tAmortization Table\n";
	cout << "*****************************************************************\n";

	cout << left
	     << setw(W_MONTH) << "Month"
	     << setw(W_BALANCE) << "Balance"
	     << setw(W_PAYMENT) << "Payment"
	     << setw(W_RATE) << "Rate"
	     << setw(W_INTEREST) << "Interest"
	     << setw(W_PRINCIPAL) << "Principal"
	     << "\n";

	cout << left
	     << setw(W_MONTH) << month
	     << setw(W_BALANCE) << money(balance)
	     << setw(W_PAYMENT) << "N/A"
	     << setw(W_RATE) << "N/A"
	     << setw(W_INTEREST) << "N/A"
	     << setw(W_PRINCIPAL) << "N/A"
	     << "\n";

	while (balance > 0.005)
	{
		month = month + 1;

		double interest = balance * monthly_rate;
		double payment;
		double principal;

		if (balance + interest < monthly_payment)
		{
			// last payment: just enough to cover what's left
			payment = balance + interest;
			principal = balance;
			balance = 0;
		}
		else
		{
			payment = monthly_payment;
			principal = payment - interest;
			balance = balance - principal;
		}

		total_interest = total_interest + interest;

		cout << left
		     << setw(W_MONTH) << month
		     << setw(W_BALANCE) << money(balance)
		     << setw(W_PAYMENT) << money(payment)
		     << setw(W_RATE) << rateStr(monthly_rate_percent)
		     << setw(W_INTEREST) << money(interest)
		     << setw(W_PRINCIPAL) << money(principal)
		     << "\n";
	}

	cout << "*****************************************************************\n";
	cout << "\nIt takes " << month << " months to pay off the loan.\n";
	cout << "Total interest paid is: " << money(total_interest) << endl;

	return 0;
}
