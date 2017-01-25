using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;

namespace SerialPortTest {
    class Program {
        static void Main(string[] args) {
            SerialPort com = new SerialPort("COM3");
            com.DataReceived += new SerialDataReceivedEventHandler(Com3_DataReceived);
            com.BaudRate = 9600;
            com.Open();
            Console.ReadKey();
        }
        private static void Com3_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            System.IO.Ports.SerialPort com = (System.IO.Ports.SerialPort)sender;
            //Console.WriteLine(com.ReadLine());
            Console.Write(com.ReadChar());
        }
    }
}
