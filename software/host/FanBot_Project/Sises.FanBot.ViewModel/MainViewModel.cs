namespace Sises.FanBot.ViewModel
{
    using System;
    using System.Collections.ObjectModel;
    using System.Windows;
    using System.Windows.Input;
    using Sises.FanBot.Model;

    public class MainViewModel : BaseViewModel
    {
        #region Fields

        private FrameViewModel currentFrame;
        private string positionString;
        
        #endregion Fields

        #region Constructors

        public MainViewModel()
        {
            this.Frames = new ObservableCollection<FrameViewModel>();

            var initialFrame = new FrameViewModel();

            this.Frames.Add(initialFrame);
            this.CurrentFrame = initialFrame;

            this.AddFrame = new RelayCommand(
                x =>
                {
                    var frame = new FrameViewModel();
                    this.Frames.Add(frame);
                    this.CurrentFrame = frame;
                },
                x => this.Frames.Count < 20 && this.Frames.IndexOf(this.CurrentFrame) == this.Frames.Count - 1);
            this.DeleteFrame = new RelayCommand(
                x =>
                {
                    var currentIndex = this.Frames.IndexOf(this.CurrentFrame);
                    this.Frames.RemoveAt(currentIndex);

                    if (currentIndex > 0) { currentIndex--; }

                    this.CurrentFrame = this.Frames[currentIndex];
                },
                x => this.Frames.Count > 1);
            this.NextFrame = new RelayCommand(
                x =>
                {
                    var currentIndex = this.Frames.IndexOf(this.CurrentFrame);
                    if (this.Frames.Count > currentIndex + 1)
                    {
                        this.CurrentFrame = this.Frames[currentIndex + 1];
                    }
                },
                x => this.Frames.Count > this.Frames.IndexOf(this.CurrentFrame) + 1);
            this.PreviousFrame = new RelayCommand(
                x =>
                {
                    var currentIndex = this.Frames.IndexOf(this.CurrentFrame);
                    if (currentIndex > 0)
                    {
                        this.CurrentFrame = this.Frames[currentIndex - 1];
                    }
                },
                x => this.Frames.IndexOf(this.CurrentFrame) > 0);
            this.Program = new RelayCommand(x => this.ProgramFanBot());
        }

        #endregion Constructors

        #region Properties

        #region Commands

        public ICommand AddFrame { get; private set; }
        public ICommand DeleteFrame { get; private set; }
        public ICommand NextFrame { get; private set; }
        public ICommand PreviousFrame { get; private set; }
        public ICommand Program { get; private set; }
        
        #endregion Commands

        public FrameViewModel CurrentFrame
        {
            get { return this.currentFrame; }
            set
            {
                if (value != this.currentFrame)
                {
                    this.currentFrame = value;
                    this.RaisePropertyChanged("CurrentFrame");

                    this.PositionString = string.Format(
                        "{0} / {1}",
                        this.Frames.IndexOf(this.CurrentFrame) + 1,
                        this.Frames.Count);
                }
            }
        }
        public ObservableCollection<FrameViewModel> Frames { get; private set; }
        public string PositionString
        {
            get { return this.positionString; }
            set
            {
                if (value != this.positionString)
                {
                    this.positionString = value;
                    this.RaisePropertyChanged("PositionString");
                }
            }
        }

        #endregion Properties
        
        #region Methods

        public void ProgramFanBot()
        {
            try
            {
                var device = HidApi.hid_open(4660, 6, null);

                if (device == IntPtr.Zero)
                {
                    MessageBox.Show("Probleem bij verbinding met FanBot", "Foutmelding");
                    return;
                }

                byte[] buffer = new byte[65];
                int i;

                HidApi.hid_flush(device);

                buffer[1] = 1; // Program command

                for (i = 0; i < this.Frames.Count; i++)
                {
                    var frame = this.Frames[i];
                    byte leds = 0;

                    if (frame.Led1Enabled) { leds += 1; }
                    if (frame.Led2Enabled) { leds += 2; }
                    if (frame.Led3Enabled) { leds += 4; }
                    if (frame.Led4Enabled) { leds += 8; }
                    if (frame.Led5Enabled) { leds += 16; }
                    if (frame.Led6Enabled) { leds += 32; }
                    if (frame.Led7Enabled) { leds += 64; }

                    buffer[2 + i * 2] = leds;
                    buffer[3 + i * 2] = BitConverter.GetBytes(frame.ServoPosition)[0];
                }

                if (i < 20)
                {
                    buffer[2 + i * 2] = 0;
                    buffer[3 + i * 2] = 0;
                }

                HidApi.hid_write(device, buffer, buffer.Length);

                HidApi.hid_close(device);
                HidApi.hid_exit();
            }
            catch (Exception)
            {
                MessageBox.Show("Er is een onbekend probleem opgetreden", "Foutmelding");
            }
        }

        #endregion Methods
    }
}