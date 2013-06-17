namespace Sises.FanBot.ViewModel
{
    using System;
    using System.ComponentModel;

    public abstract class BaseViewModel : INotifyPropertyChanged
    {
        #region Events

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion Events

        #region Methods

        protected void RaisePropertyChanged(string name)
        {
            if (string.IsNullOrEmpty(name))
            {
                throw new ArgumentException("Property name not given.");
            }

            var propertyChanged = this.PropertyChanged;

            if (propertyChanged != null)
            {
                propertyChanged(this, new PropertyChangedEventArgs(name));
            }
        }

        #endregion Methods
    }
}