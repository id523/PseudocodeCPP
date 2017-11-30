using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Assembler {
    class Program {
        static string[] OpcodeNameArray = new string[] {
	        "Ret", "Call", "TailCall", "Jump", "TrueJump", "FalseJump",
	        "BoolNot", "IntNot", "BoolAnd", "IntAnd", "BoolOr", "IntOr",
	        "StrictEqual", "StrictNeq", "NumEqual", "NumNeq",
	        "NumLt", "NumGt", "NumLeq", "NumGeq",
	        "Null", "BoolFalse", "BoolTrue", 
	        "IntLiteral", "RealLiteral", "TypeLiteral", 
	        "Add", "Sub", "Mul", "Neg", 
	        "IntDiv", "IntMod", "RealDiv", "RealMod", 
	        "ToInt", "ToReal", 
	        "GetType", 
	        "CreateObject", "ShallowCopy", "GetGlobalObject", "GetFunctionObject", 
	        "GetMember", "SetMember", "GetMemberDynamic", "SetMemberDynamic", 
	        "PushFrame", "PopFrame", "Pick", "Bury", "PopDiscard",
	        "ClearCode", "ClearText", "AppendCode", "AppendCodeLiteral", "AppendText", "AppendFormat", "PrintText",
	        "DebugLine",
        };
        static void Main(string[] args) {
            string source = null, destination = null;
            if (args.Length >= 1) source = args[0];
            if (args.Length >= 2) destination = args[1];
            if (source == null) {
                Console.WriteLine("Source file:");
                source = Console.ReadLine();
            }
            if (destination == null) {
                Console.WriteLine("Destination file:");
                destination = Console.ReadLine();
            }

        }
    }
}
