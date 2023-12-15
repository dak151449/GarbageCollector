#!/bin/sh
. gear-sh-functions

clone() {
	mkdir -p ~/git/46206 && cd ~/git/46206 || exit 2
	if [ -d "$1" ]; then
		pushd "$1" && git checkout -q sisyphus && git pull && popd
	else
		git clone git://git.altlinux.org/gears/${1:0:1}/$1
	fi
	pushd "$1" && git remote add git.alt git.alt:packages/$1 && popd
}

# 1-st argument - task number
[ $# = 1 ] || exit 3

for package in *.patch; do
  package="${package%.patch}"
  clone $package
  pushd $package
  if [ $? -ne 0 ]; then
    echo "Error: failed to cd $package"
    exit 1
  fi
  specfile=
  guess_specfile
  get_NVR_from_spec "$specfile"
  version_release="$spec_version-$spec_release"
  popd || { echo "Error: failed to popd"; exit 1; }
  if [ $? -ne 0 ]; then
  fi
  patch $package/$specfile $package.patch
  pushd $package || { echo "Error: failed to pushd $package"; exit 1; }
  git commit -a -m "Drop obsolete deps. (see also ALT#46206)"
  rc

  sed -i -r 's,^(Release:.*)$,&.1,' $specfile
  add_changelog -e "- Drop obsolete deps. (see also ALT#46206)" $specfile

  #git diff
  gear-commit -a --no-edit #&& rc

  #git clean
  #git log
  
  gear-create-tag
  firstLetter="${package:0:1}"
  version_release="$version_release.1"
  if [ "$version_release" != "$(git describe)" ]; then
    echo "Error: git describe returns $(git describe), expected $version_release"
    echo exit 1
  fi

  ssh git.alt clone /gears/$firstLetter/$package
  git push git.alt sisyphus $version_release
  ssh build.alt task add $1 repo $package $version_release
  popd || { echo "Error: failed to popd"; exit 1; }
  rm -rf $package
done
