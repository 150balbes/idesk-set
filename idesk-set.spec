Name:		idesk-set
Version:	0.0.2
Release:	alt5
Summary:	Graphical idesk-x settings

License:	GPL
Group:		Graphical desktop/Icewm
URL:		https://github.com/150balbes/idesk-x-set
Packager:	Oleg Ivanov <Leo-sp150@yandex.ru>

Source0:	%name-%version.tar.gz
Source2:	%name.desktop
Source3:	%name.png

BuildRequires: gcc-c++ libqt4-devel desktop-file-utils
Requires: qt4-styles-qtcurve

%description
Graphical application for Idesk

%prep
%setup -q
lrelease-qt4 %name.pro
DESTDIR=%buildroot PREFIX=/usr qmake-qt4 %name.pro

%build
%make_build

%install
%makeinstall
install -pD -m755 %name %buildroot%_bindir/%name
install -pD -m644 %SOURCE2 %buildroot%_desktopdir/%name.desktop
install -pD -m644 %SOURCE3 %buildroot%_liconsdir/%name.png

%files
%_bindir/%name
%_desktopdir/*
%_liconsdir/*

%changelog
* Tue Mar 09 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.2-alt5
- edit setings

* Tue Mar 04 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.2-alt4
- new ver

* Tue Mar 02 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.2-alt3
- edit del icon

* Tue Feb 29 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.2-alt2
- add del icon

* Tue Feb 28 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.2-alt1
- new version

* Tue Feb 26 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.1-alt4
- add list lnk

* Tue Feb 03 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.1-alt3
- add restart idesk

* Tue Feb 02 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.1-alt2
- add tab

* Tue Jan 26 2016 Oleg Ivanov <Leo-sp150@yandex.ru> 0.0.1-alt1
- Initial release
