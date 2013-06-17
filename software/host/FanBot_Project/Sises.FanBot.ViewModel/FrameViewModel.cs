namespace Sises.FanBot.ViewModel
{
    using System.Windows.Input;

    public class FrameViewModel : BaseViewModel
    {
        #region Fields

        private bool led1Enabled = true;
        private bool led2Enabled = true;
        private bool led3Enabled = true;
        private bool led4Enabled = true;
        private bool led5Enabled = true;
        private bool led6Enabled = true;
        private bool led7Enabled = true;
        private int servoPosition = 1;

        #endregion Fields

        #region Constructors

        public FrameViewModel()
        {
            this.ToggleLed1 = new RelayCommand(x => this.Led1Enabled = !this.Led1Enabled);
            this.ToggleLed2 = new RelayCommand(x => this.Led2Enabled = !this.Led2Enabled);
            this.ToggleLed3 = new RelayCommand(x => this.Led3Enabled = !this.Led3Enabled);
            this.ToggleLed4 = new RelayCommand(x => this.Led4Enabled = !this.Led4Enabled);
            this.ToggleLed5 = new RelayCommand(x => this.Led5Enabled = !this.Led5Enabled);
            this.ToggleLed6 = new RelayCommand(x => this.Led6Enabled = !this.Led6Enabled);
            this.ToggleLed7 = new RelayCommand(x => this.Led7Enabled = !this.Led7Enabled);
        }

        #endregion Constructors

        #region Properties

        #region Commands

        public ICommand ToggleLed1 { get; private set; }
        public ICommand ToggleLed2 { get; private set; }
        public ICommand ToggleLed3 { get; private set; }
        public ICommand ToggleLed4 { get; private set; }
        public ICommand ToggleLed5 { get; private set; }
        public ICommand ToggleLed6 { get; private set; }
        public ICommand ToggleLed7 { get; private set; }
        
        #endregion Commands

        public bool Led1Enabled
        {
            get { return this.led1Enabled; }
            set
            {
                if (value != this.led1Enabled)
                {
                    this.led1Enabled = value;
                    this.RaisePropertyChanged("Led1Enabled");
                }
            }
        }
        public bool Led2Enabled
        {
            get { return this.led2Enabled; }
            set
            {
                if (value != this.led2Enabled)
                {
                    this.led2Enabled = value;
                    this.RaisePropertyChanged("Led2Enabled");
                }
            }
        }
        public bool Led3Enabled
        {
            get { return this.led3Enabled; }
            set
            {
                if (value != this.led3Enabled)
                {
                    this.led3Enabled = value;
                    this.RaisePropertyChanged("Led3Enabled");
                }
            }
        }
        public bool Led4Enabled
        {
            get { return this.led4Enabled; }
            set
            {
                if (value != this.led4Enabled)
                {
                    this.led4Enabled = value;
                    this.RaisePropertyChanged("Led4Enabled");
                }
            }
        }
        public bool Led5Enabled
        {
            get { return this.led5Enabled; }
            set
            {
                if (value != this.led5Enabled)
                {
                    this.led5Enabled = value;
                    this.RaisePropertyChanged("Led5Enabled");
                }
            }
        }
        public bool Led6Enabled
        {
            get { return this.led6Enabled; }
            set
            {
                if (value != this.led6Enabled)
                {
                    this.led6Enabled = value;
                    this.RaisePropertyChanged("Led6Enabled");
                }
            }
        }
        public bool Led7Enabled
        {
            get { return this.led7Enabled; }
            set
            {
                if (value != this.led7Enabled)
                {
                    this.led7Enabled = value;
                    this.RaisePropertyChanged("Led7Enabled");
                }
            }
        }
        public int ServoPosition
        {
            get { return this.servoPosition; }
            set
            {
                if (value != this.servoPosition)
                {
                    this.servoPosition = value;
                    this.RaisePropertyChanged("ServoPosition");
                }
            }
        }

        #endregion Properties
    }
}