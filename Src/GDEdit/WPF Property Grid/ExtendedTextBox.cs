using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows.Input;

namespace GDEdit
{
    class ExtendedTextBox : TextBox
    {
        protected override void OnKeyDown(KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                var binding = GetBindingExpression(TextBox.TextProperty);
                binding.UpdateSource();
                e.Handled = true;
            }
        }
    }
}
