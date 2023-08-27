import subprocess

progname = "./rec7"

def ensure_built():
    makey = subprocess.getoutput("make")
    print("calling make... ", end="", flush=True)
    if "Error" in makey:
        print(makey)
        print("\n\nfailed to make; not running any tests.")
        exit()
    print("\tcode built.")

def call_it(*args):
    return subprocess.getoutput(progname + " " + " ".join(args))

def score(ans,*args):
    got = call_it(*args)
    if ans==got:
        print(".",end="",flush=True)
        return 1
    print("\nERROR:\t %s %s\n\texpected: %s\n\tgot:      %s" % (progname, " ".join(args),ans,got))
    return 0

def main():
    ensure_built()
    
    # each test has the expected answer first, then
    # the exact arguments we'd pass to ./rec7.
    tests = [
        (  "1", "clamp",  '1',  '1',  '1'),
        (  "8", "clamp",  '8',  '1', '50'),
        ( "10", "clamp", '10',  '0', '20'),
        ( "10", "clamp", '10', '10', '20'),
        ( "10", "clamp",  '5', '10', '20'),
        ( "20", "clamp", '20', '10', '20'),
        ( "20", "clamp", '25', '10', '20'),
        ( "-1", "clamp", '-3', '-1', '20'),
        ( "-1", "clamp", '-1', '-3', '20'),
        
        ("1", "bitLength",   "1"),
        ("2", "bitLength",   "2"),
        ("2", "bitLength",   "3"),
        ("3", "bitLength",   "4"),
        ("3", "bitLength",   "7"),
        ("4", "bitLength",   "9"),
        ("5", "bitLength",  "20"),
        ("7", "bitLength",  "70"),
        ("7", "bitLength", "100"),
        
        (  "0", "sumOdds", '0'),
        (  "1", "sumOdds", '1'),
        (  "4", "sumOdds", '2'),
        (  "9", "sumOdds", '3'),
        ( "16", "sumOdds", '4'),
        ( "25", "sumOdds", '5'),
        ( "36", "sumOdds", '6'),
        ( "49", "sumOdds", '7'),
        ( "64", "sumOdds", '8'),
        
        (  '0', "fib", '0'),
        (  '1', "fib", '1'),
        (  '1', "fib", '2'),
        (  '2', "fib", '3'),
        (  '3', "fib", '4'),
        (  '5', "fib", '5'),
        (  '8', "fib", '6'),
        ( '13', "fib", '7'),
        ( '21', "fib", '8'),
        
        (   '1', "collatzLength", '1'),
        (   '3', "collatzLength", '4'),
        (   '6', "collatzLength", '5'),
        (  '17', "collatzLength", '7'),
        (   '4', "collatzLength", '8'),
        (  '20', "collatzLength", '9'),
        (   '7', "collatzLength", '10'),
        (  '26', "collatzLength", '100'),
        (  '12', "collatzLength", '2048'),
        
        ]    
    
    tally = 0
    for test in tests:
        tally += score(*test)
    
    print("\n\nscore: %s/%s" % (tally,len(tests)))

if __name__=="__main__":
    main()
