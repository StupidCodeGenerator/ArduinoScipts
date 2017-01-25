using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace WindowsFormsApplication1 {

    public partial class Form1 : Form {

        public delegate void RefreshPic();

        RefreshPic refreshPic;

        SerialPort com;

        int currentXPostion = 0;

        public static int[] dataYs;

        Pen limePen = new Pen(new SolidBrush(Color.Lime));
        Pen yellowPen = new Pen(new SolidBrush(Color.Yellow));

        public Form1() {
            InitializeComponent();

            pictureBox.Paint += pictureBox_Paint;
            refreshPic = pictureBox.Refresh;
            textBoxComName.Text = "COM3";

            dataYs = new int[pictureBox.Width];
        }

        private void Com3_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e) {
            try {
                System.IO.Ports.SerialPort com = (System.IO.Ports.SerialPort)sender;
                string data = com.ReadLine().Trim();
                int dataI = int.Parse(data);
                dataYs[currentXPostion] = dataI;
                this.BeginInvoke(refreshPic);
                if (currentXPostion < dataYs.Length - 1) {
                    currentXPostion++;
                } else {
                    currentXPostion = 0;
                }
                //Console.WriteLine(dataI);
            } catch (Exception exp) {
                Console.WriteLine(exp.StackTrace.ToString());
            }
        }

        void pictureBox_Paint(object sender, PaintEventArgs e) {
            Graphics g = e.Graphics;
            g.DrawLine(yellowPen, currentXPostion, 0, currentXPostion, pictureBox.Height);
            for (int i = 0; i < dataYs.Length; i++) {
                int dataYScale = pictureBox.Height - dataYs[i];
                int lastDataYScale = pictureBox.Height - dataYs[lastXPosition(i)];
                //g.FillRectangle(new SolidBrush(Color.Lime), i, dataYScale, 1, 1);
                g.DrawLine(limePen, lastXPosition(i), lastDataYScale, i, dataYScale);
            }
            if (com != null) {
                int data = dataYs[lastXPosition(currentXPostion)];
                g.DrawString("IsOpen : " + com.IsOpen, new Font("Consolas", 8), new SolidBrush(Color.Lime), 0, 0);
                g.DrawString("Output : " + data, new Font("Consolas", 8), new SolidBrush(Color.Lime), 0, 10);
                g.DrawString("Tmp : " + data / 2, new Font("Consolas", 8), new SolidBrush(Color.Lime), 0, 20);
            }
        }

        private void buttonStart_Click(object sender, EventArgs e) {
            com = new SerialPort(textBoxComName.Text);
            com.DataReceived += new SerialDataReceivedEventHandler(Com3_DataReceived);
            com.BaudRate = 9600;
            com.Open();
        }

        int lastXPosition(int input) {
            int output = input - 1;
            if (output < 0) {
                output = 0;
            }
            return output;
        }
    }
}
