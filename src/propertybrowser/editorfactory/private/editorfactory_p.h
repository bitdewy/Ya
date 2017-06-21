/****************************************************************************
**
** BSD 3-Clause License
**
** Copyright (c) 2017, bitdewy
** All rights reserved.
**
****************************************************************************/

#pragma once

#include <QtCore/QList>
#include <QtCore/QMap>

class QObject;
class QWidget;
class Property;
template <typename Editor>
class EditorFactoryPrivate
{
public:

    using EditorList = QList<Editor*>;
    using PropertyToEditorListMap = QMap<Property*, EditorList>;
    using EditorToPropertyMap = QMap<Editor*, Property*> ;

    EditorFactoryPrivate() {}
    ~EditorFactoryPrivate() {}
    EditorFactoryPrivate(const EditorFactoryPrivate&) = delete;
    EditorFactoryPrivate& operator=(const EditorFactoryPrivate&) = delete;


    Editor* createEditor(Property* property, QWidget* parent)
    {
        auto editor = new Editor(parent);
        initializeEditor(property, editor);
        return editor;
    }

    void initializeEditor(Property* property, Editor* editor)
    {
        auto it = createdEditors_.find(property);
        if (it == createdEditors_.end())
        {
            it = createdEditors_.insert(property, EditorList());
        }
        it.value().append(editor);
        editorToProperty_.insert(editor, property);
    }

    void slotEditorDestroyed(QObject* object)
    {
        const auto ecend = editorToProperty_.end();
        for (auto itEditor = editorToProperty_.begin(); itEditor !=  ecend; ++itEditor)
        {
            if (itEditor.key() == object)
            {
                auto editor = itEditor.key();
                auto property = itEditor.value();
                const auto pit = createdEditors_.find(property);
                if (pit != createdEditors_.end())
                {
                    pit.value().removeAll(editor);
                    if (pit.value().empty())
                    {
                        createdEditors_.erase(pit);
                    }
                }
                editorToProperty_.erase(itEditor);
                return;
            }
        }
    }

    PropertyToEditorListMap createdEditors_;
    EditorToPropertyMap editorToProperty_;
};
