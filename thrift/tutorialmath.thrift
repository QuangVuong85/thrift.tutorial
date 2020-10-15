/**
* The available types in Thrift are:
*
*  bool        Boolean, one byte
*  i8 (byte)   Signed 8-bit integer
*  i16         Signed 16-bit integer
*  i32         Signed 32-bit integer
*  i64         Signed 64-bit integer
*  double      64-bit floating point value
*  string      String
*  binary      Blob (byte array)
*  map<t1,t2>  Map from one type to another
*  list<t1>    Ordered list of one type
*  set<t1>     Set of unique elements of one type
*
*/

include "sharedmath.thrift"

namespace cpp tutorialmath

typedef i32 MyInteger

const i32 INT32CONSTANT = 9853
const map<string,string> MAPCONSTANT = {'hello':'world', 'goodnight':'moon'}

enum Operation {
    ADD = 1,
    SUBTRACT = 2,
    MULTIPLY = 3,
    DIVIDE = 4
}

struct Work {
    1: i32 num1 = 0,
    2: i32 num2,
    3: Operation op,
    4: optional string comment
}

exception InvalidOperation {
    1: i32 whatOp,
    2: string why
}

service Calculator extends sharedmath.SharedService {
    void ping(),
    i32 add(1:i32 num1, 2:i32 num2),
    i32 calculate(1:i32 logid, 2:Work w) throws (1:InvalidOperation ouch),
    oneway void zip()
}