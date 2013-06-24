namespace Sises.FanBot.Model
{
    using System;
    using System.Runtime.InteropServices;

    public class HidApi
    {
        #region Methods

        #region External

        [DllImportAttribute("hidapi.dll", EntryPoint = "hid_close", CallingConvention = CallingConvention.Cdecl)]
        public static extern void hid_close(IntPtr device);

        [DllImportAttribute("hidapi.dll", EntryPoint = "hid_exit", CallingConvention = CallingConvention.Cdecl)]
        public static extern int hid_exit();

        [DllImportAttribute("hidapi.dll", EntryPoint = "hid_open", CallingConvention = CallingConvention.Cdecl)]
        public static extern System.IntPtr hid_open(ushort vendor_id, ushort product_id, string serial_number);

        [DllImportAttribute("hidapi.dll", EntryPoint = "hid_read", CallingConvention = CallingConvention.Cdecl)]
        public static extern int hid_read(IntPtr device, byte[] data, uint length);

        [DllImportAttribute("hidapi.dll", EntryPoint = "hid_read_timeout", CallingConvention = CallingConvention.Cdecl)]
        public static extern int hid_read_timeout(IntPtr device, byte[] data, uint length, int milliseconds);

        [DllImportAttribute("hidapi.dll", EntryPoint = "hid_set_nonblocking", CallingConvention = CallingConvention.Cdecl)]
        public static extern int hid_set_nonblocking(IntPtr device, int nonblock);

        [DllImportAttribute("hidapi.dll", EntryPoint = "hid_write", CallingConvention = CallingConvention.Cdecl)]
        public static extern int hid_write(IntPtr device, byte[] data, int length);

        #endregion External

        #region Internal

        public static void hid_flush(IntPtr device)
        {
            var buffer = new byte[65];

            HidApi.hid_set_nonblocking(device, 1);

            while (HidApi.hid_read(device, buffer, (uint)buffer.Length) > 0) { }

            HidApi.hid_set_nonblocking(device, 0);
        }

        #endregion Internal

        #endregion Methods

        #region Classes

        [StructLayoutAttribute(LayoutKind.Sequential)]
        public struct hid_device_info
        {
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string path;
            public ushort vendor_id;
            public ushort product_id;
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string serial_number;
            public ushort release_number;
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string manufacturer_string;
            [MarshalAsAttribute(UnmanagedType.LPStr)]
            public string product_string;
            public ushort usage_page;
            public ushort usage;
            public int interface_number;
            public System.IntPtr next;
        }

        #endregion Classes
    }
}