from crivello import get_prime


def sum_prime(M_ax):
    result = 0;
    for i in get_prime(M_ax):
        print i
        result += i
    return result


if __name__=="__main__":
    i = sum_prime(30)
    print "la somma e' %d"%i
