namespace Sises.FanBot.ViewModel
{
    using System;
    using System.Windows.Input;
    using System.Windows.Threading;

    public class RelayCommand : ICommand
    {
        #region Fields

        private readonly Predicate<object> canExecute;
        private readonly Action<object> execute;

        #endregion Fields

        #region Constructors

        public RelayCommand(Action<object> execute) : this(execute: execute, canExecute: null)
        {
        }
        public RelayCommand(Action<object> execute, Predicate<object> canExecute)
        {
            if (execute == null) { throw new ArgumentNullException("execute"); }

            this.canExecute = canExecute;
            this.execute = execute;
        }

        #endregion Constructors

        #region Events

        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        #endregion Events

        #region Methods

        #region Static
        
        public static void Refresh()
        {
            CommandManager.InvalidateRequerySuggested();
        }
        public static void Refresh(Dispatcher dispatcher)
        {
            dispatcher.Invoke(new Action(() => CommandManager.InvalidateRequerySuggested()));
        }

        #endregion Static

        public bool CanExecute(object parameter)
        {
            return this.canExecute == null ? true : this.canExecute(parameter);
        }
        public void Execute(object parameter)
        {
            this.execute(parameter);
        }

        #endregion Methods
    }
}