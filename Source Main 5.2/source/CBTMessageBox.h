#include <string>
#include <cwchar>

namespace leaf {
#if PLATFORM_WINDOWS
    int CBTMessageBox(HWND hWnd, const std::wstring& text, const std::wstring& caption, UINT uType, bool bAlwaysOnTop = false);

    class CCBTMessageBox
    {
        HWND	m_hParentWnd;
        HHOOK	m_hCBT;
        bool	m_bAlwaysOnTop;

    public:
        CCBTMessageBox();
        ~CCBTMessageBox();

        int OpenMessageBox(HWND hWnd, const wchar_t* lpText, const wchar_t* lpCaption, UINT uType, bool bAlwaysOnTop);

        HWND GetParentWndHandle() const;
        bool IsAlwaysOnTop() const;

        bool HookCBT();
        void UnhookCBT();
        HHOOK GetHookHandle() const;

        static CCBTMessageBox* GetInstance();

        static LRESULT CALLBACK CBTProc(INT nCode, WPARAM wParam, LPARAM lParam);
    };
#else
    inline int CBTMessageBox(HWND, const std::wstring& text, const std::wstring& caption, UINT, bool = false)
    {
        std::fwprintf(stderr, L"[CBTMessageBox] %ls: %ls\n", caption.c_str(), text.c_str());
        return 0;
    }

    class CCBTMessageBox
    {
    public:
        int OpenMessageBox(HWND, const wchar_t* lpText, const wchar_t* lpCaption, UINT, bool)
        {
            CBTMessageBox(nullptr, lpText ? std::wstring(lpText) : L"", lpCaption ? std::wstring(lpCaption) : L"", 0);
            return 0;
        }

        HWND GetParentWndHandle() const { return nullptr; }
        bool IsAlwaysOnTop() const { return false; }
        bool HookCBT() { return false; }
        void UnhookCBT() {}
        HHOOK GetHookHandle() const { return nullptr; }

        static CCBTMessageBox* GetInstance()
        {
            static CCBTMessageBox instance;
            return &instance;
        }
    };
#endif
}
