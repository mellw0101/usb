 while (true)
    {
        while (true)
        {
            char a;
            cout << "Do you want to try c++ native formating of ext4 filesystems (Y/N): ";
            cin >> a;
            if (a == 'Y' || a == 'y' || a == 'N' || a == 'n'){break;}
        }
        if (a == 'Y' || a == 'y')
        {
            string test = main_drive_name + "2";
            const char* ext_test1 = test.c_str();

            ext2_filsys fs;
            errcode_t result = ext2fs_initialize(ext_test1, 0, &fs);
            if (result !=)
            {
                cout << "failed to initialize '" << result << "'" << endl;
                return 1;
            }

            ext4_write_super(fs);

            result = ext2fs_create(fs, 0, 0, 0, 0, 0);
            if (result != 0)
            {
                std::cout << "Failed to create ext4 filesystem: " << result << std::endl;
                ext2fs_close_free(&fs);
                return 1;
            }

            cout <<  "Drive formatted successfully with ext4" << endl;

            ext2fs_close_free(&fs);

            system(mkfs3.c_str());
            break;
        }
        else if (a == 'N' || a == 'n'){

        }
}
