#include "pch.h"
#include "TextEditor.h"

#include <Engine/CPathMgr.h>

TextEditor::TextEditor()
	: m_textSize(0)
{
	UseMenuBar(true);
}

TextEditor::~TextEditor()
{
}

void TextEditor::Init()
{
}

void TextEditor::Update()
{
	// ���⼭ �߰� ��ư ������ �ؿ� �� �ؽ�Ʈ �Է� �����,
	// �Է��ϴ� �� ���� �ؽ�Ʈ �ڽ� 1��
	for (int i = 0; i < m_vText.size() + 1; i++)
	{
		AddText(i);
	}

	// �ؽ�Ʈ ����, �ҷ�����
	SaveLoad();
}

void TextEditor::Activate()
{

}

void TextEditor::Deactivate()
{
}

void TextEditor::AddText(int _Index)
{
	string textIndex = "TextBox" + std::to_string(_Index);
	ImGui::Text(textIndex.c_str());

	char buf[1024] = "";
	if (m_vText.size() > _Index)
	{
		strncpy_s(buf, sizeof(buf), m_vText[_Index].c_str(), _TRUNCATE);
	}

	bool changed = ImGui::InputTextMultiline(textIndex.c_str(), buf, sizeof(buf),
		ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4));

	// ���ڰ� �ٲ� ���� ����
	if (changed)
	{
		string wstr(buf);

		if (m_vText.size() <= _Index)
		{
			m_vText.push_back(wstr);
		}
		else
		{
			m_vText[_Index] = wstr;
		}
	}
}

void TextEditor::SaveLoad()
{
	if (ImGui::Button("Save##TextBox", ImVec2(50.f, 18.f)))
	{
		m_textSize = m_vText.size();

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"textBox\0*.textBox";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// Ž��â �ʱ� ��ġ ����
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"textBox\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			FILE* File = nullptr;
			_wfopen_s(&File, szSelect, L"wb");

			if (nullptr == File)
				return;

			fwrite(&m_textSize, sizeof(int), 1, File);

			for (const string& str : m_vText)
			{
				size_t strLen = str.length();
				fwrite(&strLen, sizeof(size_t), 1, File);

				if (strLen > 0)
				{
					// ���� �ϳ��� ����
					fwrite(str.c_str(), sizeof(char), strLen, File);
				}
			}

			fclose(File);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load##TextBox", ImVec2(50.f, 18.f)))
	{
		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"textBox\0*.textBox";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// Ž��â �ʱ� ��ġ ����
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"textBox\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			FILE* File = nullptr;
			_wfopen_s(&File, szSelect, L"rb");

			if (nullptr == File)
				return;

			fread(&m_textSize, sizeof(int), 1, File);
			m_vText.clear();
			m_vText.reserve(m_textSize);

			for (int i = 0; i < m_textSize; ++i)
			{
				size_t strLen = 0;
				fread(&strLen, sizeof(size_t), 1, File);

				if (strLen > 0)
				{
					vector<char> buffer(strLen + 1, 0); 
					fread(buffer.data(), sizeof(char), strLen, File);
					m_vText.push_back(string(buffer.data()));
				}
				// ������ �������
				else
				{
					m_vText.push_back("");
				}
			}

			fclose(File);
		}
		else
		{
			return;
		}
	}
}
