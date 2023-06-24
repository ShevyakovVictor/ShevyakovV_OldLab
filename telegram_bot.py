import asyncio
import telegram

async def main():
    bot = telegram.Bot("5663800264:AAF4IH2K87sMdBrXK9cVFNsaKiImdErXGTk")
    async with bot:
        print(await bot.get_me())


if __name__ == '__main__':
    asyncio.run(main())