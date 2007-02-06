%define prefix /usr
%define bindir %{prefix}/bin
%define sysconfdir %{prefix}/share
%define docdir %{prefix}/share/wxmacmolplt
%define debug_package %{nil}
%define __check_files %{nil}
Name: wxmacmolplt
Version: 6.3.0
Release: 1
Summary: MacMolPlt molecular visualization package
Group: undefined
License: GPL
Prefix: %{prefix}
Source: wxmacmolplt-%{version}.tar.gz  
BuildRoot: %{_tmppath}/wxmacmolplt-%{version}-root
BuildRequires: wxGTK-devel >= 2.6.3
Requires: wxGTK >= 2.6.3 wxGTK-gl >= 2.6.3

%description
wxMacMolPlt is a molecular visualization tool for use with GAMESS

%prep
%setup
%define CONFIG_FLAGS --prefix=%{prefix} --enable-server --enable-clients

%files
%defattr(-, root, root)
%{bindir}/wxmacmolplt
%{sysconfdir}/wxmacmolplt/MacMolPlt.Prefs.xml
%{sysconfdir}/wxmacmolplt/arial1.glf
%doc %{sysconfdir}/doc/wxmacmolplt/LICENSE
%doc %{sysconfdir}/man/man1/wxmacmolplt.1.gz
%doc %{docdir}/MacMolPlt_Manual.html
%doc %{docdir}/Manual_pages/*
%doc %{docdir}/samples/*

%build
./configure %{CONFIG_FLAGS}
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -r $RPM_BUILD_ROOT; fi;
make DESTDIR=$RPM_BUILD_ROOT prefix=%{prefix} install
