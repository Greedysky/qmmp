#!/bin/sh

LOCALES='en ru uk_UA zh_CN zh_TW tr cs pt_BR pt de pl_PL fr it kk lt hu nl ja sk es he gl_ES sr_BA sr_RS bg el'

echo "Locales: ${LOCALES}"

for tr_dir in `find ../src/ -type d -name "translations"`
do

     plug_name=`echo $tr_dir | cut -d '/' -f 5`

     if  [ -z "$plug_name" ]; then
         plug_name=`echo $tr_dir | cut -d '/' -f 3`
     fi

     echo "Updating ${plug_name}"

     case $plug_name in
        IncDecVolumeOption)
            plug_name="incdecvolume"
        ;;
        PlayListOption)
            plug_name="playlist"
        ;;
        SeekOption)
            plug_name="seek"
        ;;
        StatusOption)
            plug_name="status"
        ;;
        UninstallOption)
            plug_name="uninstall"
        ;;
        QmmpFileDialog)
            plug_name="qmmp_file_dialog"
        ;;
        TwoPanelFileDialog)
            plug_name="two_panel_file_dialog"
        ;;
        qmmpui)
            plug_name="libqmmpui"
        ;;
        app)
            plug_name="qmmp"
        ;;
     esac

     ts_files=''
     qm_files=''
     if [ "$plug_name" = "qmmp" ] || [ "$plug_name" = "libqmmpui" ]; then
        for code in $LOCALES
        do
      	    ts_files="${ts_files} ${tr_dir}/${plug_name}_${code}.ts"
            if [ "${code}" != "en" ]; then
              qm_files="${qm_files} ${plug_name}_${code}.qm"
            fi
      	done
        qrc_file="${tr_dir}/${plug_name}_locales.qrc"
     else
        for code in $LOCALES
        do
      	    ts_files="${ts_files} ${tr_dir}/${plug_name}_plugin_${code}.ts"
            if [ "${code}" != "en" ]; then
              qm_files="${qm_files} ${plug_name}_plugin_${code}.qm"
            fi
      	done
        qrc_file="${tr_dir}/translations.qrc"
     fi

     lupdate-qt4 -no-obsolete -silent -extensions "cpp,ui" ${tr_dir}/../ -ts ${ts_files}



     echo "<!DOCTYPE RCC>" > $qrc_file
     echo "<RCC version=\"1.0\">" >> $qrc_file
     echo "  <qresource>" >> $qrc_file
     for qm_file in $qm_files
     do
         echo "    <file>${qm_file}</file>" >> $qrc_file;
     done
     echo "  </qresource>" >> $qrc_file
     echo "</RCC>" >> $qrc_file
done

echo "Updating text resources..."

qrc_file="../src/qmmpui/txt/txt.qrc"
echo "<!DOCTYPE RCC>" > $qrc_file
echo "<RCC version=\"1.0\">" >> $qrc_file
echo "  <qresource>" >> $qrc_file
echo "    <file>../../../COPYING</file>" >> $qrc_file
echo "" >> $qrc_file

echo "    <file>description.txt</file>" >> $qrc_file
echo "    <file>authors.txt</file>" >> $qrc_file
echo "    <file>thanks.txt</file>" >> $qrc_file
echo "    <file>translators.txt</file>" >> $qrc_file
echo "" >> $qrc_file

for txt_file in `find ../src/qmmpui/txt -type f -name "authors_*"`
do
	locale=`echo $txt_file | cut -d '_' -f 2,3 | cut -d '.' -f 1`
	echo "Adding ${locale}"
	echo "    <file>description_${locale}.txt</file>" >> $qrc_file
    echo "    <file>authors_${locale}.txt</file>" >> $qrc_file
    echo "    <file>thanks_${locale}.txt</file>" >> $qrc_file
    echo "    <file>translators_${locale}.txt</file>" >> $qrc_file
    echo "" >> $qrc_file
done

echo "  </qresource>" >> $qrc_file
echo "</RCC>" >> $qrc_file
