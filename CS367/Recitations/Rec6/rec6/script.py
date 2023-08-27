
import subprocess

progname = "./rec6"

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
    # the exact arguments we'd pass to ./rec6.
    tests = [
        ("120","mul_24","5"),
        ("144","mul_24","6"),
        ("-24","mul_24","-1"),
        ("2400","mul_24","100"),
        
        ( "1","average", '0', '2'),
        ("10","average",'10','10'),
        ("12","average",'11','13'),
        ("15","average",'10','20'),

        ( '0', "diff_sq", '1', '1'),
        ( '12', "diff_sq", '4', '2'),
        (  '9', "diff_sq", '-5', '4'),
        ( '10000', "diff_sq", '100', '0'),
        ( '-60', "diff_sq", '2', '8'),
        
        ( "101 (0x65)", "set_bit",  '100', '0' ),
        ( "68 (0x44)", "set_bit",  '0x44', '2'),
        ( "20 (0x14)", "set_bit",  '0x14', '2'),
        ( "28 (0x1C)", "set_bit",  '0x14', '3'),
        
        ( "0 [101 (0x65)]", "set_bit_param",  '100', '0'),
        ( "1 [68 (0x44)]", "set_bit_param",  '0x44', '2'),
        ( "1 [20 (0x14)]", "set_bit_param",  '0x14', '2'),
        ( "0 [28 (0x1C)]", "set_bit_param",  '0x14', '3'),
        
        ( "0 [70 (0x46)]", "set_bit_val", '0x44', '1', '1'),
        ( "1 [68 (0x44)]", "set_bit_val", '0x44', '2', '1'),
        ( "0 [68 (0x44)]", "set_bit_val", '0x44', '5', '0'),
        ( "1 [4 (0x4)]", "set_bit_val", '0x44', '6', '0'),
        
        ]    
    
    tally = 0
    for test in tests:
        tally += score(*test)
    
    print("\n\nscore: %s/%s" % (tally,len(tests)))

if __name__=="__main__":
    main()
