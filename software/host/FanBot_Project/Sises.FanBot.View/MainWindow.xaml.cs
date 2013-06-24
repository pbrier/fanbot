namespace Sises.FanBot.View
{
    using System.Windows;
    using Sises.FanBot.ViewModel;

    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = new MainViewModel();
        }
    }
}