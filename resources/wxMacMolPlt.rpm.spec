#%define prefix /usr
#%define bindir %{prefix}/bin
#%define sysconfdir %{prefix}/share
#%define docdir %{prefix}/share/wxmacmolplt
#%define debug_package %{nil}
#%define __check_files %{nil}
Name: wxmacmolplt
Version: 7.6.1
Release: 1%{?dist}
Summary: A graphics program for plotting 3-D molecular structures and normal modes
Group: Applications/Engineering
License: GPLv2+
URL: http://brettbode.github.io/wxmacmolplt/
#Prefix: %{prefix}
#Source: wxmacmolplt-%{version}.tar.gz  
Source: https://bintray.com/artifact/download/brettbode/generic/wxmacmolplt-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
#BuildRoot: %{_tmppath}/wxmacmolplt-%{version}-root
BuildRequires: desktop-file-utils
BuildRequires: glew-devel
BuildRequires: wxGTK-devel >= 2.6.3
#FC
Requires: hicolor-icon-theme
#Requires: wxGTK >= 2.6.3 wxGTK-gl >= 2.6.3
#Mandriva 2007.1 naming
#Requires: libwxgtk2.6 >= 2.6.3 libming0

%description
MacMolPlt is:
* A modern graphics program for plotting 3-D molecular structures and
  normal modes (vibrations). Modern means:
  o Mouse driven interface for real-time rotation and translation.
  o copy and paste functionality for interfacing to other programs such
    as word processors or other graphics programs (like ChemDraw).
  o simple printing to color or black and white printers (publication
    quality).
  o multiple files open at once.
* It reads a variety of file formats including any GAMESS input, log or
  IRC file directly to create animations of IRC's, DRC's, and
  optimizations. You may also import a $VEC group from any file (such as
  a GAMESS .DAT file). In addition xMol XYZ files, MolDen format files
  and Chemical Markup Language (CML) files are supported. Also some PDB
  file support and MDL MolFile support is included.

%prep
%setup -q
%define CONFIG_FLAGS --prefix=%{prefix} --enable-server --enable-clients

%files
%defattr(-, root, root,-)
%doc LICENSE
%{_bindir}/wxmacmolplt
%{_mandir}/man1/wxmacmolplt.1*
%{_datadir}/applications/*.desktop
%{_datadir}/icons/hicolor/128x128/apps/wxmacmolplt.png
%{_datadir}/wxmacmolplt

#%{sysconfdir}/wxmacmolplt/MacMolPlt.Prefs.xml
#%{sysconfdir}/wxmacmolplt/arial1.glf
#%{sysconfdir}/wxmacmolplt/arial1.bmf
#%{sysconfdir}/wxmacmolplt/organics.cml
#%{sysconfdir}/wxmacmolplt/solvents.cml
#%{sysconfdir}/wxmacmolplt/amino_acids.cml
#%{sysconfdir}/wxmacmolplt/splash.jpg
#%doc %{sysconfdir}/doc/wxmacmolplt/LICENSE
#FC gz
#%doc %{sysconfdir}/man/man1/wxmacmolplt.1.gz
#Mandriva uses bz2
#%doc %{sysconfdir}/man/man1/wxmacmolplt.1.bz2
#%doc %{docdir}/MacMolPlt_Manual.html
#%doc %{docdir}/Manual_pages/*
#%doc %{docdir}/samples/*

%build
%configure --docdir=%{_datadir}/doc/%{name}-%{version}
make %{?_smp_mflags}
#./configure %{CONFIG_FLAGS}
#make

%install
rm -rf %{buildroot}
make DESTDIR=%{buildroot} install
install -Dpm644 resources/wxmacmolplt.png %{buildroot}%{_datadir}/icons/hicolor/128x128/apps/wxmacmolplt.png
desktop-file-install --dir=%{buildroot}%{_datadir}/applications resources/wxmacmolplt.desktop
#if [ -d $RPM_BUILD_ROOT ]; then rm -r $RPM_BUILD_ROOT; fi;
#make DESTDIR=$RPM_BUILD_ROOT prefix=%{prefix} install

%clean
rm -rf %{buildroot}

%post
touch --no-create %{_datadir}/icons/hicolor &>/dev/null || :

%postun
if [ $1 -eq 0 ] ; then
    touch --no-create %{_datadir}/icons/hicolor &>/dev/null
    gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :
fi

%posttrans
gtk-update-icon-cache %{_datadir}/icons/hicolor &>/dev/null || :

%changelog
* Thu Dec 09 2010 Brett Bode <brett@scl.ameslab.gov> 7.4.2
- Updated the included spec file from Dominik Mierzejewski
